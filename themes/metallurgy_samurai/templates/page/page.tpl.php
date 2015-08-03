<?php
/**
 * @file page.tpl.php
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['header']: Items for the header region.
 * - $page['featured']: Items for the featured region.
 * - $page['sidebar_right']: Items for the sidebar first region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<div class="row">
<section id="header-wrapper">
  <div class="row">
  <div id="header" class="col s12 m12 l10 offset-l2">
    <h1>Samurai</h1>
    <h3><?php print $title ?></h3>
  </div>
</div>
</section>
<section id="menu-wrapper" class="col s2 hide-on-med-and-down">
  <div id="menu">
    <?php print theme('links__system_main_menu', array(
      'links' => $main_menu,
      'attributes' => array(
        'id' => 'main-menu',
        'class' => array(
          'links',
        ),
      ), 
      'heading' => t('Navigation'),
    )); ?>
  </div>
  <div id="menu">
    <?php print theme('links__system_secondary_menu', array(
      'links' => $secondary_menu,
      'attributes' => array(
        'id' => 'main-menu',
        'class' => array(
          'links'
          ),
        ),
        'heading' => t('User menu'),
      )); ?>
  </div>
</section>
<section id="menu-mobile-wrapper" class="hide-on-large-only">
    <a href="#" id="menu-open"><i class="mdi-navigation-menu"></i></a>
    <div id="menu">
      <?php print theme('links__system_main_menu', array(
        'links' => $main_menu,
        'attributes' => array(
          'id' => 'main-menu',
          'class' => array(
            'links',
          ),
        ),
        'heading' => t('Navigation'),
      )); ?>
      <?php print theme('links__system_secondary_menu', array(
        'links' => $secondary_menu,
        'attributes' => array(
          'id' => 'main-menu',
          'class' => array(
            'links'
            ),
          ),
        'heading' => t('User menu'),
        )); ?>
    </div>  
</section>
<section id="content-wrapper" class="col s12 m12 l10 offset-l2">
  <div id="content">
    <?php if (!empty($messages)): ?>
      <?php print $messages; ?>
    <?php endif; ?>
    <?php if (!empty($tabs)): ?>
      <?php print render($tabs); ?>
    <?php endif; ?>
    <?php if (!empty($action_links)): ?>
      <div class="action-links">
        <?php print render($action_links); ?>
      </div>
    <?php endif; ?>
    <?php print render($page['content']); ?>
    <div class="fixed-action-btn" style="bottom: 45px; right: 24px;">
      <a class="btn-floating btn-large samurai-blue waves-effect waves-light">
        <i class="large material-icons">add</i>
      </a>
      <ul>
        <li>
          <a onclick="location.href='/client-site/config/add'"class="btn-floating white tooltipped" data-position="left" data-delay="0" data-tooltip="Add site"><i class="material-icons" style="color: #333;">playlist_add</i></a>
        </li>
        <li>
          <a onclick="location.href='/client-site/config'"class="btn-floating white tooltipped" data-position="left" data-delay="0" data-tooltip="Manage sites"><i class="material-icons" style="color: #333;">device_hub</i></a>
        </li>
      </ul>
  </div>
  </div>
</section>
</div>
<div id="sidenav-overlay" style="opacity: 0; z-index: -10;"></div>