<?php
/**
 * @file
 * Theme functions for metallurgy_samurai
 */


include drupal_get_path('theme', 'metallurgy_samurai') . '/includes/menu.inc';
include drupal_get_path('theme', 'metallurgy_samurai') . '/includes/theme.inc';

function metallurgy_samurai_theme($existing, $type, $theme, $path) {
  return array(
    'security_announcement' => array(
      'variables' => array('element' => null),
      'template' => 'templates/entity/entity--sa'
    ),
    'client_site' => array(
      'variables' => array('element' => null),
      'template' => 'templates/entity/entity--client-site'
    ),
  );
}
?>
<!DOCTYPE html>
  <html>
    <head>
      <!--Import materialize.css-->
      <link type="text/css" rel="stylesheet" href="sites/all/libraries/materialize/css/materialize.min.css"  media="screen,projection"/>

      <!--Let browser know website is optimized for mobile-->
      <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    </head>

    <body>
      <!--Import jQuery before materialize.js-->
      <script type="text/javascript" src="https://code.jquery.com/jquery-2.1.1.min.js"></script>
      <script type="text/javascript" src="sites/all/libraries/materialize/js/materialize.min.js"></script>
    </body>
  </html>
  <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
  <div class="fixed-action-btn" style="bottom: 45px; right: 24px;">
    <a class="btn-floating btn-large red">
      <i onclick="location.href='site/config/add'" class="large material-icons">add</i>
    </a>
    <ul>
      <li><a class="btn-floating green"><i onclick="location.href='site'" class="material-icons">code</i></i></a></li>
    </ul>
  </div>
