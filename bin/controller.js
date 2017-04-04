'use strict'; angular.module('microscope')
    .controller('LedController', ['$scope', '$http', function($scope, $http){
	$scope.stat = {value: 0};
	$scope.blueval = 500;
	$scope.greenval = 500;
	$scope.focusMode = 'COARSE';

	/*but1 = document.getElementById('but1');
	but2 = document.getElementById('but2');
	but3 = document.getElementById('but3');
	but4 = document.getElementById('but4');
	but5 = document.getElementById('but5');
	text1 = document.getElementById('text1');
	text2 = document.getElementById('text2');
	lv = document.getElementById('live-video');
	sv = document.getElementById('stop-video');
	startvc = document.getElementById('startvc');
	stopvc = document.getElementById('stopvc');
	*/

	$scope.initial = function(){
		document.getElementById('but3').disabled = true;
		document.getElementById('but5').disabled = true;
		document.getElementById('text1').disabled = true;
		document.getElementById('text2').disabled = true;
	};

	$scope.initial();

	$scope.captureImage = function(){
		console.log("Image captured")
		$http.post('/api/capture');
	}

	$scope.liveVideo = function(status){
		$scope.stat.status = status
		$http.post('/api/liveVideo', $scope.stat).then(
		function successCallback(response){
			if(status){
				document.getElementById('live-video').style.display = "none";
				document.getElementById('stop-video').style.display = "";
				document.getElementById('but3').disabled = false;
				document.getElementById('but5').disabled = false;
				document.getElementById('text1').disabled = false;
				document.getElementById('text2').disabled = false;
			}
			else{
				document.getElementById('live-video').style.display = "";
				document.getElementById('stop-video').style.display = "none";
				document.getElementById('but3').disabled = true;
				document.getElementById('but5').disabled = true;
				document.getElementById('text1').disabled = true;
				document.getElementById('text2').disabled = true;

			}

		},
		function errorCallback(response){
			if(status){
				document.getElementById('live-video').style.display = "none";
				document.getElementById('stop-video').style.display = "";
			}
			else{
				document.getElementById('live-video').style.display = "";
				document.getElementById('stop-video').style.display = "none";
			}
		});
	}

	$scope.captureVideo = function(status){
		$scope.stat.capStatus = status
		$scope.stat.interval = $scope.interval
		$scope.stat.repeat = $scope.repeat
		$scope.stat.intensity = document.getElementById('slider').value;
		if((typeof($scope.interval) == typeof(2)) && (typeof($scope.repeat) == typeof(2))){
			$http.post('/api/captureVideo', $scope.stat).then(
			function successCallback(response){
				if(status){
					document.getElementById("startvc").style.display = "none";
					document.getElementById("stopvc").style.display = "";
					document.getElementById('live-video').style.display = "none";
					document.getElementById('but2').disabled = true;
					document.getElementById('but5').disabled = true;
				}
				else{
					document.getElementById("startvc").style.display = "";
					document.getElementById("stopvc").style.display = "none";
					document.getElementById('live-video').style.display = "";
					document.getElementById('stop-video').style.display = "none";
					document.getElementById('but5').disabled = true;
				}
			},
			function errorCallback(response){
				if(status){
					document.getElementById("startvc").style.display = "none";
					document.getElementById("stopvc").style.display = "";
					document.getElementById('live-video').style.display = "";
					document.getElementById('stop-video').style.display = "none";
				}
				else{
					document.getElementById("startvc").style.display = "";
					document.getElementById("stopvc").style.display = "none";
					document.getElementById('live-video').style.display = "";
					document.getElementById('stop-video').style.display = "none";
				}
			});
		}
		else{
			alert("Enter correct values");
		}
	}

	$scope.downloadImages = function(){
		var win = window.open('/api/downloadImages','','width=, height=, resizable=no');
                win.resizeTo(0,0);
                win.moveTo(0,window.screen.availHeight+10);
	};

	$scope.downloadTimeLapse = function(){
		var win = window.open('/api/downloadTimeLapse','','width=, height=, resizable=no');
		win.resizeTo(0,0);
		win.moveTo(0,window.screen.availHeight+10);
	};

	$scope.changeFocus = function(){
		console.log("Getting in change focus");
		if($scope.focusMode == 'FINE'){
			document.getElementById('focus-fine').style.display = "none";
			document.getElementById('focus-coarse').style.display = "";
			$scope.focusMode = 'COARSE'; 
		}
		else if($scope.focusMode == 'COARSE'){
			document.getElementById('focus-fine').style.display = "";
                        document.getElementById('focus-coarse').style.display = "none";
			$scope.focusMode = 'FINE';
		}
	};

    }])
	.controller('ImageController', ['$scope', '$http', function($scope, $http){
		$scope.$on('$stateChangeSuccess', function(event, toState, toParams, fromState, fromParams){
			$http.get('/api/getImages').then(
				function successCall(response){
					$scope.filenames = response.data;
				},
				function errorCall(response){
				}
			);
		})
	}])
