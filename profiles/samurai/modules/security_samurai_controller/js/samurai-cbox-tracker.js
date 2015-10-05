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

      if (getVal() == true) {
        donechecked = false;
      } else {
        donechecked = true
      }

      $("label[for='edit-master-checkbox']").on("click", function() {
        if (getVal() == true && donechecked == false) {
          donechecked = true;
          Materialize.toast('Auto-updates are now enabled', 3000);
        }
        if (getVal() == false && donechecked == true) {
          donechecked = false;
          Materialize.toast('Auto-updates are now disabled', 3000);
        }
      });
    }
  };
})(jQuery);
