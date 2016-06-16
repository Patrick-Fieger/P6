// Resize mit langer Pause zwischendrin
!function(n,r){var t=function(n,r,t){var e;return function(){function i(){t||n.apply(u,a),e=null}var u=this,a=arguments;e?clearTimeout(e):t&&n.apply(u,a),e=setTimeout(i,r||1000)}};jQuery.fn[r]=function(n){return n?this.bind("resize",t(n)):this.trigger(r)}}(jQuery,"smartresizelong");

$(document).ready(function() {

	var lorem = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmodtempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodoconsequat. Duis aute irure dolor in reprehenderit in voluptate velit essecillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat nonproident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	function init(){
		$('canvas').constellation({});
	}

	$(window).smartresizelong(function(){
		$('canvas').constellation({});
	});


	$(document).on('click', "[href*='#']", function(event) {
		event.preventDefault();
		var h = $(this).attr('href');

		if(h.charAt(0) == "#" && h.length > 1){
			$('html,body').animate({scrollTop : $(h).offset().top}, 700);
		}
	});

	$('section .text').each(function(index, el) {
		for (var i = 0; i < 6; i++) {
			$(this).append('<p>' + lorem + '<p>');
		}
	});

	$('.menu a').each(function(index, el) {
		$('nav ul').append($(this)[0].outerHTML);
	});

	init();
});