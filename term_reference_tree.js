(function($) {
  Drupal.behaviors.term_reference_tree = {
    attach: function(context, settings){
      $('.checkbox_tree_plus', context).click(function(){
        if($(this).hasClass('plus')){
          if($(this).siblings('ul').first().length){
            $(this).siblings('ul').first().show();
            $(this).removeClass('plus');
            $(this).addClass('minus');
          } else {
            var original_click = $(this);
            var tid = $(this).siblings('label').first().children().first().val();
            var field_id = $(this).attr('id').split('__').shift();
            $.getJSON(Drupal.settings.term_reference_tree.callback+'/'+tid+'/'+field_id, function(data){
              $(original_click).parent().append(data[1]["data"]);
              Drupal.attachBehaviors($(original_click).parent().children());
              $(original_click).removeClass('plus');
              $(original_click).addClass('minus');
            });
          }
        } else {
          $(this).siblings('ul').first().hide();    
          $(this).removeClass('minus');
          $(this).addClass('plus');    
        }
      });
    }
  };
})(jQuery);