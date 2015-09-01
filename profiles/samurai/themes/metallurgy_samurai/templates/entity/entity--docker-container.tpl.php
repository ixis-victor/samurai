<div class="row">
  <div class="col s12 m8 l8">
    <div id="content-main-wrapper">
      <div id="content-main">
        <h4 class="publish-info card">Created: <?php print date('d-m-Y H:i:s', $element->creation_date); ?></h4>
        <?php if (!is_null($element->container_description)): ?>
          <div class="card popup">
            <div class="card-content">
              <span class="card-title">Environment description</span>
              <p><?php print render($element->container_description); ?></p>
            </div>
          </div>
        <?php endif; ?>
        <div class="card popup">
          <div class="card-content">
            <span class="card-title">Environment information</span>
            <?php
              // Load the associated client site entity
              $client_site = client_site_load($element->associated_site_id);
            ?>
            <p><strong>Site: </strong><a style="text-decoration: underline;" href="/client-site/<?php print render($element->associated_site_id); ?>"><?php print render($client_site->name); ?></a></p>
            <p><strong>Branch: </strong><?php print render($element->container_branch); ?></p>
            <p><strong>Environment ID: </strong><?php print render($element->container_id); ?></p>
            <p><strong>Environment base image: </strong><?php print render($element->image_name); ?></p>
            <p><strong>Environment URL: </strong><?php print samurai_docker_get_accessible_url($element->id); ?></p>
          </div>
        </div>
      </div>
    </div>
  </div>
  <div id="column-right-wrapper" class="col s12 m4 l4">
    <div id="column-right">
      <div class="card popup">
        <div class="card-content">
          <span class="card-title">Environment actions</span>
          <p class="content">Stop environment</p>
          <p class="content">Export environment</p>
          <p class="content">Delete environment</p>
        </div>
      </div>
    </div>
  </div>
</div>
