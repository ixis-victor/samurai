<div class="row">
  <div class="col s12 m8 l8">
    <div id="content-main-wrapper">
      <div id="content-main">
        <h4 class="publish-info card">Created: <?php print date('d-m-Y H:i:s', $element->creation_date); ?></h4>
        <?php if (!is_null($element->container_description)): ?>
          <div class="card popup">
            <div class="card-content">
              <span class="card-title">About</span>
              <p><?php print render($element->container_description); ?></p>
            </div>
          </div>
        <?php endif; ?>
        <?php
          // Load the associated client site entity
          $client_site = client_site_load($element->associated_site_id);
        ?>
        <table class="card popup">
          <thead>
            <tr>
              <th>Environment information</th>
            </tr>
          </thead>
          <tbody>
            <tr><td>Site</td><td><?php print l($client_site->name, '/client-site/' . $element->associated_site_id); ?></td></tr>
            <tr><td>Active branch</td><td><?php print render($element->container_branch); ?></td></tr>
            <tr><td>Environment URL</td><td><?php print l(samurai_docker_get_accessible_url($element->id), samurai_docker_get_accessible_url($element->id)); ?></td></tr>
            <tr><td>Environment ID</td><td><?php print render($element->container_id); ?></td></tr>
            <tr><td>Environment base image</td><td><?php print render($element->image_name); ?></td></tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
  <div id="column-right-wrapper" class="col s12 m4 l4">
    <div id="column-right">
      <div class="card popup">
        <div class="card-content">
          <span class="card-title"></span>
          <?php $container_actions_form = drupal_get_form('samurai_docker_container_actions_form'); ?>
          <?php print render($container_actions_form); ?>
        </div>
      </div>
    </div>
  </div>
</div>
