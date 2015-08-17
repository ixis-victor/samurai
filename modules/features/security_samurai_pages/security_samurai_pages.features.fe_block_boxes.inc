<?php
/**
 * @file
 * security_samurai_pages.features.fe_block_boxes.inc
 */

/**
 * Implements hook_default_fe_block_boxes().
 */
function security_samurai_pages_default_fe_block_boxes() {
  $export = array();

  $fe_block_boxes = new stdClass();
  $fe_block_boxes->info = 'Environment & image actions';
  $fe_block_boxes->format = 'filtered_html';
  $fe_block_boxes->machine_name = 'environment_image_actions';
  $fe_block_boxes->body = '<a href="/jobs/environments/config/add">Create a new environment</a>
<a href="/jobs/images/config/add">Create a new image</a>';

  $export['environment_image_actions'] = $fe_block_boxes;

  return $export;
}
