angular
	.module('microscope',[
		'ui.router'
	])
	.config(['$urlRouterProvider', '$stateProvider', function($urlRouterProvider, $stateProvider){
		$urlRouterProvider.otherwise('/');

		$stateProvider
			.state('home', {
				url: '/',
				templateUrl: 'resources/html/home.html',
				controller: 'LedController'
			})
			.state('images', {
				url: '/images',
				templateUrl: 'resources/html/image-gallery.html',
				controller: 'ImageController'
			})
	}])

