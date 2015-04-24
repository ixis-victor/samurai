<?php
/**
 * @file
 * Theme functions for metallurgy_samurai
 */


include drupal_get_path('theme', 'metallurgy_samurai') . '/includes/menu.inc';

function metallurgy_samurai_theme($existing, $type, $theme, $path) {
  return array(
    'security_announcement' => array(
      'variables' => array('element' => null),
      'template' => 'templates/entity/entity-type-sa'
    ),
    'client' => array(
      'variables' => array('element' => null),
      'template' => 'templates/entity/entity-type-client'
    ),
  );
}