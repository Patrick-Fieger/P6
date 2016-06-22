// Resize mit langer Pause zwischendrin
!function(n,r){var t=function(n,r,t){var e;return function(){function i(){t||n.apply(u,a),e=null}var u=this,a=arguments;e?clearTimeout(e):t&&n.apply(u,a),e=setTimeout(i,r||1000)}};jQuery.fn[r]=function(n){return n?this.bind("resize",t(n)):this.trigger(r)}}(jQuery,"smartresizelong");

$(document).ready(function() {

	// window.onbeforeunload = function(){
	// 	window.scrollTo(0,0);
	// }

	var lorem = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmodtempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodoconsequat. Duis aute irure dolor in reprehenderit in voluptate velit essecillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat nonproident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	var scroll_positions = [];
	var window_scroll_position = 0;
	var actual_index = -1;
	var scroll_speed = 700;
	var devmode = true;

	function c(string){
		if(devmode) console.log(string);
	}

	function a(string){
		if(devmode) alert(string);
	}

	function init(){
		initBGs();
		initVoice();
		setTimeout(function() { initScrollPositions(); }, 300);

		initMenu();
		$('canvas').constellation({});
	}

	function initBGs(){
		$('[data-bg]').each(function(index, el) {
			c($(this).data('bg'));
			$(this).css({'background-image':'url(' + $(this).data('bg') + ')'});
			$(this).append('<div class="icon-resize-full-screen show_preview"></div>')
		});
	}

	function initMenu(){
		$('section h1').each(function(index, el) {
			if(index > 0){
				$('.menu').append('<h2><a href="#'+$(this).closest('section').attr('id')+'">'+ index + '. ' +  $(this).text()) + '</a></h2>';
			}
		});
	}

	function initScrollPositions(){
		$('section').each(function () {
			scroll_positions.push({
				offset : $(this).offset().top,
				height : $(this).outerHeight(true),
				id : $(this).attr('id'),
				headline : $(this).find('h1').text()
			});
		}).promise().done(function(){
			$(window).scroll(function() {
				window_scroll_position = $(window).scrollTop();

				for (var j = 0; j < scroll_positions.length; j++) {
					if(window_scroll_position >= scroll_positions[j].offset && window_scroll_position < (scroll_positions[j].offset + scroll_positions[j].height)){
						actual_index = j;
					}
				}
			});

			$(window).trigger('scroll');
		})
	}

	function initVoice(){
		var recognition = new webkitSpeechRecognition();
		recognition.continuous = true;
		recognition.interimResults = true;
		var woerter = [
			['inhalt','#inhalt'],
			['über uns','#us'],
			['management','#management'],
			['space','#space']
		];

		recognition.start();

		recognition.onstart = function() { c('start')}
		recognition.onerror = function(event) {c(event) }
		recognition.onresult = function(event) {
			var wort = event.results[0][0].transcript.toLowerCase();
			for (var i = event.resultIndex; i < event.results.length; ++i) {
				if(event.results[i].isFinal){
					for (var k = 0; k < woerter.length; k++) {
						var index = woerter[k][0].indexOf(wort);

						if(wort == "weiter" || wort == "walter" || wort == "weißer"){
							$('html,body').animate({scrollTop : scroll_positions[actual_index+1].offset + 2}, scroll_speed);
						}else if(wort == "zurück"){
							if(actual_index == 0){
								$('html,body').animate({scrollTop : scroll_positions[actual_index].offset + 2}, scroll_speed);
							}else{
								$('html,body').animate({scrollTop : scroll_positions[actual_index-1].offset + 2}, scroll_speed);
							}
						}else if(index > -1){
							$('html,body').animate({scrollTop : $(woerter[k][1]).offset().top + 2}, scroll_speed);
						}

						c(index);
						c(wort);
						c(actual_index);
					}
				}
			}

			recognition.stop();
		}
		recognition.onend = function() {
			c('ende!')
			recognition.start();
		}
	}

	$(window).smartresizelong(function(){
		$('canvas').constellation({});
	});


	$(document).on('click', "[href*='#']", function(event) {
		event.preventDefault();
		var h = $(this).attr('href');

		if(h.charAt(0) == "#" && h.length > 1){
			$('html,body').animate({scrollTop : $(h).offset().top + 2}, scroll_speed);
		}
	});

	$(document).on('click', '.show_preview', function(event) {
		event.preventDefault();
		var bg = $(this).closest('.trenner').attr('data-bg');

		$('.preview').css('background-image','url('+bg+'').addClass('active');
	});

	$(document).on('click', '.close_preview', function(event) {
		event.preventDefault();
		$('.preview').removeClass('active');
		setTimeout(function() {
			$('.preview').removeAttr('style')
		}, 500);
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