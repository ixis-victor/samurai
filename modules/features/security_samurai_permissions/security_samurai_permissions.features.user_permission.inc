<?php
/**
 * @file
 * security_samurai_permissions.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function security_samurai_permissions_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer security samurai'.
  $permissions['administer security samurai'] = array(
    'name' => 'administer security samurai',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'security_controller_samurai',
  );

  // Exported permission: 'administer security samurai clients'.
  $permissions['administer security samurai clients'] = array(
    'name' => 'administer security samurai clients',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'security_controller_samurai',
  );

  // Exported permission: 'view security samurai update status'.
  $permissions['view security samurai update status'] = array(
    'name' => 'view security samurai update status',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'security_controller_samurai',
  );

  return $permissions;
}
