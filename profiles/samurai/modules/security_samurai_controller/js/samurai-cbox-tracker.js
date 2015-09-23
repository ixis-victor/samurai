(function ($) {

  Drupal.behaviors.samurai_cbox_tracker_init = {
    attach: function(context, settings) {
      // On page load - set the project checkboxes to
      // disable/enable depending on the value of the
      // master checkbox

      $(".samurai-cbox-tracker").prop("disabled", getVal());

      $("label[for='edit-master-checkbox']").on("click", function() {
        $(".samurai-cbox-tracker").prop("disabled", getVal());
      });

      function getVal() {
        if ($("#edit-master-checkbox").is(":checked")) {
          return false;
        } else {
          return true;
        }
      }
    }
  };
})(jQuery);
