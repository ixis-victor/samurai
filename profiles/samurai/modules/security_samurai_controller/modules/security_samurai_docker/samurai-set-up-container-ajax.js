(function ($) {
  /**
   * Load remote content after the main page loaded.
   */
  Drupal.behaviors.samurai_load_remote_content = {
    attach: function(context, settings) {
      $('#samurai-remote-wrapper').once('samurai-remote-wrapper', function() {
        var base = $(this).attr('id');


        var element_settings = {
          url: 'http://' + window.location.hostname +  settings.basePath + settings.pathPrefix + 'ajax/remote/environment',
          event: 'click',
          progress: {
            type: 'throbber'
          }
        };
        Drupal.ajax[base] = new Drupal.ajax(base, this, element_settings);
        $(this).click();
      });
    }
  };
})(jQuery);