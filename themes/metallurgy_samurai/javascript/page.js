/*
 * Page related JS code and functions.
 */

// // Execute when page is ready.
// (jQuery)(document).ready(function() {
// 	// If the scroll position of the page is at a certain position,
// 	// do the magic.
//   var elementExists = document.getElementById("title");
//   if (elementExists != null) {
//     var animated = 0;
//     document.addEventListener("scroll", function() {
//       // Assign the scroll position of the page.
//       var top = window.scrollY;
//       // Check the scroll position of the page.
//       if (top > 60) {
//         if (animated == 0) {
//           animated = 1;
//           // Animate the header title
//           (jQuery)("#content-header").css({
//             "z-index": "100",
//             "margin-top": "-200px",
//             "position": "fixed",
//             "font-size": "85%"
//           });
//           (jQuery)("section#content-wrapper").css({
//             "margin-top": "130px",
//           });
//           // Animate the header into view.
//           (jQuery)("#content-header").animate({
//             "margin-top": "-143px"
//           }, 200);
//         }
//       }
//       // Return the content header back to it's original state.
//       if (top < 60) {
//         if (animated == 1) {
//           animated = 0;
//           window.setTimeout(function() {
//             (jQuery)("section#content-wrapper").css({
//               "margin-top": "55px",
//             });
//             // Animate the header title
//             (jQuery)("#content-header").css({
//               "z-index": "-1",
//               "margin-top": "-90px",
//               "position": "relative",
//               "font-size": "100%",
//               "margin-top": "0.3em"
//             });
//           }, 0);
//         }
//       }
//     });
//   }
// });

// // Execute when page is ready.
// (jQuery)(document).ready(function() {
//   // This code opens and closes the menu
//   var toggleopen = 0;
//   (jQuery)("#menu-open").click(function() {
//       // Hide the menu toggle and push out the menu.
//       (jQuery)("#menu-open").animate({
//         "opacity": "0"
//       }, 200);
//       (jQuery)("#menu-wrapper").animate({
//         "width": "60%",
//       }, 200);
//       (jQuery)("#mobile-menu").animate({
//         "margin-left": "0px",
//         "padding-right": "0px"
//       }, 200);
//       (jQuery)("#menu-close").animate({
//         "opacity": "1"
//       }, 200);
//   });

//   (jQuery)("#menu-close").click(function() {
//     // Hide the menu toggle and push in the menu.
//     (jQuery)("#menu-close").animate({
//       "opacity": "0"
//     }, 200);
//     (jQuery)("#mobile-menu").animate({
//       "margin-left": "-150px",
//       "padding-right": "50px"
//     }, 200);
//     (jQuery)("#menu-wrapper").animate({
//       "width": "50px"
//     }, 200);
//     (jQuery)("#menu-open").animate({
//       "opacity": "1"
//     }, 200);
//   })
// });