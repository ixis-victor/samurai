<div id="left-content">
  <div id="content-header-wrapper">
    <div id="content-header">
      <h1 id="title"><?php print $element->name; ?></h1>
      <h1 id="title-type">Client site</h1>
    </div>
  </div>
  <div id="content-main-wrapper">
    <div id="content-main">
      <h4 class="publish-info">Created: <?php print date('d-m-Y H:i:s', $element->created_at); ?></h4>
      <div class="card">
        <h2 class="title">Site information</h2>
        <?php if ($element->update_next_check > 0): ?>
          <p class="content">The site will be scanned for new projects automatically at <?php print date('d-m-Y H:i:s', $element->update_next_check); ?>.</p>
        <?php else: ?>
          <p class="content">The site will be scanned on the next cron run (less than 5 minutes).</p>
        <?php endif; ?>
      </div>
      <!-- print a theme grid right here. -->
      <?php $grid_data = samurai_render_update_data($element->site_update_data); ?>
      <?php if (!is_null($grid_data)): ?>
        <table class="client-projects">
          <thead>
            <tr>
              <th>Name</th>
              <th>Type</th>
              <th>Installed version</th>
              <th>Latest version</th>
              <th>Latest secure version</th>
            </tr>
          </thead>
        <?php foreach($grid_data as $data): ?>
          <tr>
            <td><?php print $data['name']; ?></td>
            <td><?php print $data['type']; ?></td>
            <td <?php 
              if (isset($data['status'])) {
                print 'class="' . $data['status'] . '"';
              }
            ?>><?php print $data['installed_version']; ?></td>
            <td><?php print $data['latest_version']; ?></td>
            <td><?php print $data['secure_version']; ?></td>
          </tr>
        <?php endforeach; ?>
        </table>
      <?php else: ?>
        <div class="card">
          <h2 class="title">404 - No projects found :(</h2>
          <p class="content">Doesn't look like any projects for this client exist. Try scanning the site.</p>
        </div>
      <?php endif; ?>
    </div>
  </div>
</div>
<div id="right-content">
  <div id="column-right-wrapper">
    <div id="column-right">
      <!-- Print the block manually -->
      <div class="card">
        <h2 class="title">Scan the site now</h2>
        <p class="content">button here</p>
      </div>
    </div>
  </div>
</div>