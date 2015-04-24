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
        <p class="content">The site will be scanned for new projects at <?php print date('d-m-Y H:i:s', $element->update_next_check); ?></p>
      </div>
    </div>
  </div>
</div>