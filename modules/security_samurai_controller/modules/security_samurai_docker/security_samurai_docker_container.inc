<?php
/**
 * @file
 * security_samurai_docker_container.inc
 * Configuration and management forms for the docker_container entity.
 */

/**
 * Implements drupal_form()
 */
function docker_container_form($form, &$form_state) {

	$form = array();

  $form['docker_container_form_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The container name, so people know what it\'s being used for.'),
  );

  $form['docker_container_form_site'] = array(
    '#type' => 'select',
    '#title' => t('Site to use'),
    '#decsription' => t('The site to set up on the container, all Git configuration is carried over from the site.'),
    '#options' => samurai_get_sites(),
  );

  $form['docker_container_form_branch'] = array(
    '#type' => 'select',
    '#title' => t('Branch to deploy from'),
    '#description' => t('The git branch to deploy the code from to the new container.'),
    // Will need to be done via ajax.
    // '#options' => samurai_gitapi_get_branches
    '#options' => array(
      '1',
    ),
  );

  $form['docker_container_form_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );

  $form['#submit'][] = 'docker_container_form_submit';

  return $form;
}

/**
 * Form submit handler for @see docker_container_form()
 */
function docker_container_form_submit($form_id, &$form_state) {

  $docker = new Docker;
  // Example: $docker->execute_command("f249d15672b5", "ls -lash");
  // Docker PHP API needs finishing before moving any more forward with this.
}