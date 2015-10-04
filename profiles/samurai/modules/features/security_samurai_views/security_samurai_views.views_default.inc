<?php
/**
 * @file
 * security_samurai_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function security_samurai_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'environments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'securitysamurai_dockercontainers';
  $view->human_name = 'Environments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Environments';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'container_branch' => 'container_branch',
    'container_id' => 'container_id',
    'container_name' => 'container_name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'container_branch' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'container_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'container_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<h1 class="center">No environments!</h1>
<p class="center">Add an environment by clicking \'Create a new environment\'</p>';
  /* Relationship: Environment: Associated site ID */
  $handler->display->display_options['relationships']['associated_site_id']['id'] = 'associated_site_id';
  $handler->display->display_options['relationships']['associated_site_id']['table'] = 'securitysamurai_dockercontainers';
  $handler->display->display_options['relationships']['associated_site_id']['field'] = 'associated_site_id';
  /* Field: Environment: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'securitysamurai_dockercontainers';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Environment: Environment name */
  $handler->display->display_options['fields']['container_name']['id'] = 'container_name';
  $handler->display->display_options['fields']['container_name']['table'] = 'securitysamurai_dockercontainers';
  $handler->display->display_options['fields']['container_name']['field'] = 'container_name';
  $handler->display->display_options['fields']['container_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['container_name']['alter']['text'] = '<a style="text-decoration: underline;" href="/jobs/environments/[id]">[container_name]</a>';
  /* Field: Site: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'securitysamurai_site';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'associated_site_id';
  $handler->display->display_options['fields']['name']['label'] = 'Site name';
  /* Field: Environment: Environment branch */
  $handler->display->display_options['fields']['container_branch']['id'] = 'container_branch';
  $handler->display->display_options['fields']['container_branch']['table'] = 'securitysamurai_dockercontainers';
  $handler->display->display_options['fields']['container_branch']['field'] = 'container_branch';
  $handler->display->display_options['fields']['container_branch']['label'] = 'Branch';
  /* Filter criterion: Environment: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'securitysamurai_dockercontainers';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['status']['group_info']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['group_info']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Active',
      'operator' => '=',
      'value' => '1',
    ),
    2 => array(
      'title' => 'Inactive',
      'operator' => '=',
      'value' => '0',
    ),
  );
  /* Filter criterion: Site: Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'securitysamurai_site';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['relationship'] = 'associated_site_id';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Site name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['filters']['name']['group_info']['label'] = 'Name';
  $handler->display->display_options['filters']['name']['group_info']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['group_info']['group_items'] = array(
    1 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
  );
  /* Filter criterion: Environment: Environment name */
  $handler->display->display_options['filters']['container_name']['id'] = 'container_name';
  $handler->display->display_options['filters']['container_name']['table'] = 'securitysamurai_dockercontainers';
  $handler->display->display_options['filters']['container_name']['field'] = 'container_name';
  $handler->display->display_options['filters']['container_name']['operator'] = 'contains';
  $handler->display->display_options['filters']['container_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['container_name']['expose']['operator_id'] = 'container_name_op';
  $handler->display->display_options['filters']['container_name']['expose']['label'] = 'Environment name';
  $handler->display->display_options['filters']['container_name']['expose']['operator'] = 'container_name_op';
  $handler->display->display_options['filters']['container_name']['expose']['identifier'] = 'container_name';
  $handler->display->display_options['filters']['container_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $export['environments'] = $view;

  $view = new view();
  $view->name = 'security_announcements';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'securitysamurai_sa';
  $view->human_name = 'Security announcements';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Security announcements';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '6';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Security announcement: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Security announcement: SA ID */
  $handler->display->display_options['fields']['sa_id']['id'] = 'sa_id';
  $handler->display->display_options['fields']['sa_id']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['sa_id']['field'] = 'sa_id';
  $handler->display->display_options['fields']['sa_id']['label'] = '';
  $handler->display->display_options['fields']['sa_id']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['sa_id']['alter']['path'] = 'security-announcement/[id]';
  $handler->display->display_options['fields']['sa_id']['element_label_colon'] = FALSE;
  /* Field: Security announcement: Affected project */
  $handler->display->display_options['fields']['project_name']['id'] = 'project_name';
  $handler->display->display_options['fields']['project_name']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['project_name']['field'] = 'project_name';
  /* Field: Security announcement: Risk level */
  $handler->display->display_options['fields']['risk_level']['id'] = 'risk_level';
  $handler->display->display_options['fields']['risk_level']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['risk_level']['field'] = 'risk_level';
  $handler->display->display_options['fields']['risk_level']['label'] = '';
  $handler->display->display_options['fields']['risk_level']['element_label_colon'] = FALSE;
  /* Sort criterion: Security announcement: Issue date */
  $handler->display->display_options['sorts']['date']['id'] = 'date';
  $handler->display->display_options['sorts']['date']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['sorts']['date']['field'] = 'date';
  $handler->display->display_options['sorts']['date']['order'] = 'DESC';
  /* Filter criterion: Security announcement: Risk level */
  $handler->display->display_options['filters']['risk_level']['id'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['filters']['risk_level']['field'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['operator'] = 'contains';
  $handler->display->display_options['filters']['risk_level']['exposed'] = TRUE;
  $handler->display->display_options['filters']['risk_level']['expose']['operator_id'] = 'risk_level_op';
  $handler->display->display_options['filters']['risk_level']['expose']['label'] = 'Risk level';
  $handler->display->display_options['filters']['risk_level']['expose']['operator'] = 'risk_level_op';
  $handler->display->display_options['filters']['risk_level']['expose']['identifier'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['risk_level']['group_info']['label'] = 'Risk_level';
  $handler->display->display_options['filters']['risk_level']['group_info']['identifier'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['risk_level']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );

  /* Display: Security announcement overview */
  $handler = $view->new_display('panel_pane', 'Security announcement overview', 'securityannouncement_panel_1');
  $handler->display->display_options['display_description'] = 'Front page security announcement overview.';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $export['security_announcements'] = $view;

  $view = new view();
  $view->name = 'sites';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'securitysamurai_site';
  $view->human_name = 'Sites';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Site overview';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'name' => 'name',
    'update_count' => 'update_count',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'update_count' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<h1 class="center">No sites!</h1>
<p class="center">Add a site by clicking on the plus in the bottom right corner</p>';
  /* Field: Site: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'securitysamurai_site';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  /* Field: Site: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'securitysamurai_site';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<a href="/client-site/[id]">[name]</a>';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Site: Update count */
  $handler->display->display_options['fields']['update_count']['id'] = 'update_count';
  $handler->display->display_options['fields']['update_count']['table'] = 'securitysamurai_site';
  $handler->display->display_options['fields']['update_count']['field'] = 'update_count';
  $handler->display->display_options['fields']['update_count']['label'] = 'Available updates';
  $handler->display->display_options['fields']['update_count']['element_label_colon'] = FALSE;
  /* Sort criterion: Site: Update count */
  $handler->display->display_options['sorts']['update_count']['id'] = 'update_count';
  $handler->display->display_options['sorts']['update_count']['table'] = 'securitysamurai_site';
  $handler->display->display_options['sorts']['update_count']['field'] = 'update_count';
  $handler->display->display_options['sorts']['update_count']['order'] = 'DESC';

  /* Display: Site overview */
  $handler = $view->new_display('panel_pane', 'Site overview', 'site_overview_panel_1');
  $handler->display->display_options['display_description'] = 'Front page site overview.';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'name' => 'name',
    'update_count' => 'update_count',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'update_count' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Unfiltered text */
  $handler->display->display_options['footer']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['footer']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['content'] = '<a href="/site">See more</a>';

  /* Display: Site list */
  $handler = $view->new_display('panel_pane', 'Site list', 'site_list_panel_1');
  $handler->display->display_options['display_description'] = 'Site list with pager and filters.';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Site: Core major version */
  $handler->display->display_options['filters']['core_version']['id'] = 'core_version';
  $handler->display->display_options['filters']['core_version']['table'] = 'securitysamurai_site';
  $handler->display->display_options['filters']['core_version']['field'] = 'core_version';
  $handler->display->display_options['filters']['core_version']['exposed'] = TRUE;
  $handler->display->display_options['filters']['core_version']['expose']['operator_id'] = 'core_version_op';
  $handler->display->display_options['filters']['core_version']['expose']['label'] = 'Core major version';
  $handler->display->display_options['filters']['core_version']['expose']['operator'] = 'core_version_op';
  $handler->display->display_options['filters']['core_version']['expose']['identifier'] = 'core_version';
  $handler->display->display_options['filters']['core_version']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['core_version']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['core_version']['group_info']['label'] = 'Drupal core version';
  $handler->display->display_options['filters']['core_version']['group_info']['identifier'] = 'core_version';
  $handler->display->display_options['filters']['core_version']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Drupal 8',
      'operator' => '=',
      'value' => '8.x',
    ),
    2 => array(
      'title' => 'Drupal 7',
      'operator' => '=',
      'value' => '7.x',
    ),
    3 => array(
      'title' => 'Drupal 6',
      'operator' => '=',
      'value' => '6.x',
    ),
  );
  $export['sites'] = $view;

  return $export;
}
