<?php
/**
 * @file
 * security_samurai_docker.admin.inc
 * Administration funcions
 */

/**
 * Implements hook_form
 */
function security_samurai_docker_admin_config_form($form, &$form_state, $client = NULL) {

  $form = array();

  // Max amount of docker containers
  $form['samurai_docker_max_containers'] = array(
    '#title' => t('Max Docker containers'),
    '#description' => t('The max number of Docker containers that can be running at any one time.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('samurai_docker_max_containers', '3'),
  );

  // Maybe it would be better to do this per client site?
  $form['samurai_docker_post_installation_configuration'] = array(
  	'#title' => t('Post-installation configuration script'),
  	'#description' => t('An address for your repo that will run custom post installation commands. Advanced users only.')
  	'#type' => 'textfield',
  	'#default_value' => variable_get('samurai_docker_post_installation_configuration', ''),
  );

  $form = system_settings_form($form);

  return $form;
}