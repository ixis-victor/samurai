/*
 * Page related JS code and functions.
 */
(jQuery)(document).ready(function() {

	var menu_open = 0;

	(jQuery)("#menu-open").click(function() {
		if (menu_open == 0) {
			// Open the menu
			(jQuery)("#menu-mobile-wrapper #menu").animate({
				"margin-left": "0px",
			}, 250);
			(jQuery)("#sidenav-overlay").animate({
				"z-index": "997",
				"opacity": "1",
			}, 250);
			menu_open = 1;
		}
	});

	(jQuery)("#sidenav-overlay").click(function() {
		if (menu_open == 1) {
			// Close the menu
			(jQuery)("#menu-mobile-wrapper #menu").animate({
				"margin-left": "-300px",
			}, 250);
			(jQuery)("#sidenav-overlay").animate({
				"z-index": "-10",
				"opacity": "0",
			}, 250);
			menu_open = 0;
		}
	});
});