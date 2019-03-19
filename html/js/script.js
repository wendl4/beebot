var steps = "";

function sendSteps(inputSteps) {
	$.ajax({
		url: "/sendsteps.cgi",
		type: "post",
		data: { steps:inputSteps },
		success: function(odp) {
			steps = "";
			console.log("success");
		},
		cache: false,
		error: function () { alert('Nastala chyba.'); }
	});
}

$(document).ready(function(){
	$( ".down-arrow" ).click(function() {
		steps += "d";
	});
	$( ".up-arrow" ).click(function() {
		steps += "u";
	});
	$( ".left-arrow" ).click(function() {
		steps += "l";
	});
	$( ".right-arrow" ).click(function() {
		steps += "r";
	});
	$( ".go-button" ).click(function() {
		sendSteps(steps);
	});
});
