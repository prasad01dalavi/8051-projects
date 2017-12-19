(function ($) {

$(function() {
  var googleCSEWatermark = function(id) {
    var f = $(id)[0];
    if (f && (f.query || f['edit-search-block-form--2'] || f['edit-keys'])) {
      var q = f.query ? f.query : (f['edit-search-block-form--2'] ? f['edit-search-block-form--2'] : f['edit-keys']);
      var n = navigator;
      var l = location;
      if (n.platform == 'Win32') {
        q.style.cssText = 'border: 1px solid #7e9db9; padding: 2px;';
      }
      var b = function() {
        if (q.value == '') {
          q.style.background = '#FFFFFF url(https://www.google.com/cse/intl/' + Drupal.settings.googleCSE.language + '/images/google_custom_search_watermark.gif) left no-repeat';
        }
      };
      var f = function() {
        q.style.background = '#ffffff';
      };
      q.onfocus = f;
      q.onblur = b;
//      if (!/[&?]query=[^&]/.test(l.search)) {
        b();
//      }
    }
  };
  googleCSEWatermark('#search-block-form.google-cse');
  googleCSEWatermark('#search-form.google-cse');
  googleCSEWatermark('#google-cse-results-searchbox-form');
});

})(jQuery);
;
(function ($) {

Drupal.behaviors.commentNotify = {
  attach: function (context) {
    $('#edit-notify', context)
      .bind('change', function() {
        $('#edit-notify-type', context)
          [this.checked ? 'show' : 'hide']()
          .find('input[type=checkbox]:checked').attr('checked', 'checked');
      })
      .trigger('change');
  }
}

})(jQuery);
;
