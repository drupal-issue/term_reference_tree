(function($) {
  Drupal.behaviors.term_reference_tree = {
    attach: function(context, settings){
      $('.options_tree_plus', context).click(function(){
        if($(this).hasClass('plus')){
          if($(this).siblings('ul').first().length){
            $(this).siblings('ul').first().slideDown();
            $(this).removeClass('plus').addClass('minus');
          } else {
            var original_click = $(this);
            $.getJSON(Drupal.settings.term_reference_tree.callback+'/'+$(this).siblings('label').first().children('input').first().val()+'/'+$(this).attr('id').split('__').shift(), function(data){
              $(original_click).parent().append(data[1]["data"]);
              Drupal.attachBehaviors($(original_click).parent().children());
              $(original_click).removeClass('plus').addClass('minus');
            });
          }
        } else {
          $(this).siblings('ul').first().slideUp();
          $(this).removeClass('minus').addClass('plus');
        }
      });
    }
  };
})(jQuery);

/*(function($) {
  $(function() {
    $('.term-reference-tree-button').click(function() {
      $(this).toggleClass('term-reference-tree-collapsed');
      $(this).siblings('ul').slideToggle('fast');
    });
    
    $('.expandbutton').click(function() {
      $(this).siblings('.term-reference-tree-button').trigger('click');
    });
     
    $('.term-reference-tree').each(function() {
      var tree = $(this);
      checkMaxChoices(tree);
      $(this).find('input[type=checkbox]').change(function() {
        checkMaxChoices(tree);
      });
      
      if($(this).hasClass('term-reference-tree-start-minimized')) {
        $(this).find('.term-reference-tree-button').addClass('term-reference-tree-collapsed').siblings('ul').hide();
      }
    });
  });
  
  function checkMaxChoices(item) {
    var maxChoices = item.attr('data-max-choices');
    var count = item.find(':checked').length;
    
    if(maxChoices > 0 && count >= maxChoices) {
      item.find('input[type=checkbox]:not(:checked)').attr('disabled', 'disabled').parent().addClass('disabled');
    } else {
      item.find('input[type=checkbox]').removeAttr('disabled').parent().removeClass('disabled');
    }
  }
})(jQuery);*/