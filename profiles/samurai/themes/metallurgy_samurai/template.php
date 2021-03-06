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
    'docker_containers' => array(
      'variables' => array('element' => null),
      'template' => 'templates/entity/entity--docker-container'
    ),
  );
}

function metallurgy_samurai_preprocess_html(&$variables) {
  drupal_add_css(libraries_get_path('materialize') . '/css/materialize.min.css');
  drupal_add_js(libraries_get_path('materialize') . '/js/materialize.min.js');
}

function metallurgy_samurai_css_alter(&$css) {
  // Remove defaults.css file.
  unset($css[drupal_get_path('module', 'system') . '/system.theme.css']);
}

function metallurgy_samurai_form_alter(&$form, &$form_state, $form_id) {
  // If form id is user_profile_form
  if ($form_id == 'user_profile_form') {
    $form['account']['#type'] = 'fieldset';
    $form['account']['#title'] = t('User information');
  }
}
