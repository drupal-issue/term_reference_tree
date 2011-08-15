 <?php

/**
 * Implements hook_field_formatter_settings_summary().
 */
function term_reference_tree_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  if ($display['type'] == 'term_reference_tree') {
    $summary = t('Uses tokens: ') . t($settings['token_display_selected'] != '' ? 'Yes' : 'No');
  }

  return $summary;
}


/**
 * Implements hook_field_formatter_info().
 */
function term_reference_tree_field_formatter_info() {
  return array(
    'term_reference_tree' => array(
      'label' => 'Term reference tree',
      'field types' => array('taxonomy_term_reference'),
      'settings' => array(
        'token_display_selected' => '',
        'token_display_unselected' => '',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function term_reference_tree_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'term_reference_tree':
      $element[] = array(
        '#theme' => 'term_tree_list',
        '#data' => $items,
        '#display' => $display,
        '#attached' => array('css' => array(drupal_get_path('module', 'term_reference_tree') . '/term_reference_tree.css')),
      );
      break;
  }

  return $element;
}

/**
 * Themes the term tree display (as opposed to the select widget).
 */
function theme_term_tree_list($variables) {
  $element =& $variables['element'];
  $data =& $element['#data'];

  $tree = array();

  # For each selected term:
  foreach($data as $item) {
    # Loop if the term ID is not zero:
    $values = array();
    $tid = $item['tid'];
    $original_tid = $tid;
    while($tid != 0) {
      # Unshift the term onto an array
      array_unshift($values, $tid);

      # Repeat with parent term
      $tid = _term_reference_tree_get_parent($tid);
    }

    $current =& $tree;
    # For each term in the above array:
    foreach($values as $tid) {
      # current[children][term_id] = new array
      if(!isset($current['children'][$tid])) {
        $current['children'][$tid] = array('selected' => FALSE);
      }

      # If this is the last value in the array, tree[children][term_id][selected] = true
      if($tid == $original_tid) {
        $current['children'][$tid]['selected'] = TRUE;
      }

      $current['children'][$tid]['tid'] = $tid;
      $current =& $current['children'][$tid];
    }
  }

  return _term_reference_tree_output_list_level($element, $tree);
}

/**
 * Helper function to output a single level of the term reference tree
 * display.
 */
function _term_reference_tree_output_list_level(&$element, &$tree) {
  if(isset($tree['children']) && is_array($tree['children']) && count($tree['children'] > 0)) {
    $output = '<ul class="term">';
    $settings = $element['#display']['settings'];
    $tokens_selected = $settings['token_display_selected'];
    $tokens_unselected = ($settings['token_display_unselected'] != '') ? $settings['token_display_unselected'] : $tokens_selected;

    foreach($tree['children'] as &$item) {
      $term = taxonomy_term_load($item['tid']);
      $uri = taxonomy_term_uri($term);
      $class = $item['selected'] ? 'selected' : 'unselected';
      $output .= "<li class='$class'>";
      if($tokens_selected != '' && module_exists('token')) {
        $replace = $item['selected'] ? $tokens_selected : $tokens_unselected;
        $output .= token_replace($replace, array('term' => $term), array('clear' => TRUE));
      }
      else {
        $output .= l($term->name, $uri['path'], array('html' => true));
      }
      if(isset($item['children'])) {
        $output .= _term_reference_tree_output_list_level($element, $item);
      }
      $output .= "</li>";
    }

    $output .= '</ul>';
    return $output;
  }
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function term_reference_tree_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();
  if($display['type'] == 'term_reference_tree' && module_exists('token')) {
    $element['token_display_selected'] = array(
      '#type' => 'textarea',
      '#title' => 'Custom Term Label',
      '#description' => t("Use tokens to change the term label.  Leave this field blank to use the term name linked to its taxonomy page."),
      '#default_value' => $settings['token_display_selected'],
    );

    $element['token_display_unselected'] = array(
      '#type' => 'textarea',
      '#title' => 'Custom Term Label (unselected)',
      '#description' => t("Use tokens to change the term label for unselected parent terms.  Leave this field blank to use the same tokens as above."),
      '#default_value' => $settings['token_display_unselected'],
    );

    $element['tokens_list'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('term'),
    );
  }

  return $element;
}


