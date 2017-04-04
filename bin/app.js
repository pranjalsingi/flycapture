angular
	.module('microscope',[
		'ui.router'
	])
	.config(['$urlRouterProvider', '$stateProvider', function($urlRouterProvider, $stateProvider){
		$urlRouterProvider.otherwise('/');

		$stateProvider
			.state('home', {
				url: '/',
				templateUrl: 'bin/templates/home.html',
				controller: 'LedController'
			})
			.state('images', {
				url: '/images',
				templateUrl: 'bin/templates/images.html',
				controller: 'ImageController'
			})
	}])

