Package.describe({
  name: 'dennispark:owl-carousel-1',
  version: '0.0.1',
  summary: 'Owl Carousel 1.3.2 packaged for Meteor',
  git: 'https://github.com/InchoonPark/meteor-owl-carousel-1.git',
  documentation: 'README.md'
});

Package.onUse(function(api) {
  api.versionsFrom('1.2.1');
  api.use('jquery', 'client');
  api.addFiles([
    'lib/owl.carousel.min.js',
    'lib/owl.carousel.css',
    'lib/owl.theme.css',
    'lib/owl.transitions.css'
  ], 'client');
});
