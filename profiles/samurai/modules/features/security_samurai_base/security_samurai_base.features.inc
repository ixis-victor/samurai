<?php
/**
 * @file
 * security_samurai_base.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function security_samurai_base_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "elysia_cron" && $api == "default_elysia_cron_rules") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}