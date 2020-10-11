var app = angular.module('myApp', [
    'ngRoute',
    'mobile-angular-ui',
	'btford.socket-io',
  'ngResource',
  'zingchart-angularjs',
  'ui.bootstrap'
]).config(function($routeProvider) {
    $routeProvider.when('/', {
        templateUrl: 'home.html',
        controller: 'Home'
    });
}).factory('mySocket', function (socketFactory) {
	var myIoSocket = io.connect('/webapp');	//Tên namespace webapp

	mySocket = socketFactory({
		ioSocket: myIoSocket
	});
	return mySocket;

/////////////////////// Những dòng code ở trên phần này là phần cài đặt, các bạn hãy đọc thêm về angularjs để hiểu, cái này không nhảy cóc được nha!
}).factory('resource', function($resource) {

})

app.controller('Home', function($scope, $http, $uibModal, mySocket) {
  $scope.open = function() {
    var modalInstance =  $uibModal.open({
      templateUrl: "modal.html",
      controller: "ModalContentCtrl",
      size: '',
    });
  };

  var init = function() {
    mySocket.emit("test", "test");
    var model_request = {
      method: 'GET',
      url: "http://127.0.0.1:8000/modelList/",
    }
    $http(model_request)
      .then(function(response) {
        var options = []
        for(i=0;i<response.data.length;i++) {
          var model = {
            name: response.data[i]
          }
          options.push(model)
        }
        $scope.selectableModels = {
          model: null,
          availableOptions: options
         };
    });
  }

  init();

  $scope.spectrumGraphData = []
  $scope.connection_status = { value: 'Disconnect' }
  $scope.spectrum = {value : ''}
  $scope.graphJson = {
    title: {
      text: "Spectrum Graph"
    },
    scaleY: {
      values: '0:1000:100'
    }
  }
  $scope.getData  = function() {
    var json = {
      "getData": "Get Data"
    }
    mySocket.emit("getData", json);
	}

  $scope.PLSprediction = function() {
    var data = {
      "nir": $scope.spectrum.value
    }
    var plsrequest = {
      method: 'POST',
      url: "http://127.0.0.1:8000/plsRegression/",
      data: data
    }
    $http(plsrequest)
      .then(function(response) {
      $scope.result = "PLSRegression predict: " + response.data + " days";
  });
  }

  $scope.NeuralNetworkprediction = function() {
    var data = {
      "nir": $scope.spectrum.value,
      "model": $scope.selectableModels.model
    }
    var annrequest = {
      method: 'POST',
      url: "http://127.0.0.1:8000/ann/",
      data: data
    }
    $http(annrequest)
      .then(function(response) {
      $scope.result = "Neural Network predict: " + response.data
  });
  }

	//// Khu 4 -- Những dòng code sẽ được thực thi khi kết nối với Arduino (thông qua socket server)
	mySocket.on('connect', function() {
		console.log("connected")
	})

  mySocket.on('spectrum', function(json) {
    $scope.spectrum.value = json.data.data
    var myTextArea = $('#myTextarea');
    myTextArea.val(myTextArea.val() + '\n' + $scope.spectrum.value);
    $scope.spectrumGraphData= json.data.data.split(",").map(function(item) {
    return parseInt(item, 10);
});
  })

  mySocket.on('esp8266status', function(json) {
    $scope.connection_status.value = json.data
  })

  mySocket.on('connected', function(json) {
    $scope.connection_status.value = "Connected"
  })
});

app.controller('ModalContentCtrl', function($scope, $uibModalInstance, $http, $route) {

  var uploadUrl = "http://127.0.0.1:8000/uploadModel/"
  $scope.import = function(){
    var fd = new FormData();
    //Take the first selected file
    fd.append("dataFile", document.getElementById('dataFile').files[0]);
    fd.append("modelFile", document.getElementById('modelFile').files[0]);
    fd.append("weightFile", document.getElementById('weightFile').files[0]);
    fd.append("modelName", $scope.modelName);
    fd.append("modelType", $scope.modelType);

    $http.post(uploadUrl, fd, {
        withCredentials: false,
        headers: {'Content-Type': undefined },
        transformRequest: angular.identity
    }).then(function(response) {
      $route.reload();
    });
    $uibModalInstance.dismiss();
  }

  $scope.cancel = function(){
    $uibModalInstance.dismiss();
  }

});
