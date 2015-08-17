<?php
/**
 * @file
 * security_samurai_docker_image.inc
 * Configuration and management forms for the docker_image entity.
 */

// Docker PHP API needs finishing before moving on with this.

function docker_image_form($form, &$form_state) {

	$form = array();

  $form['docker_image_container'] = array(
    '#type' => 'fieldset',
    '#title' => 'Basic information',
    '#collapsed' => FALSE,
    '#fieldset_icon' => 'camera',
  );

  return $form;
}