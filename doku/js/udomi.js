// Resize mit langer Pause zwischendrin
!function(n,r){var t=function(n,r,t){var e;return function(){function i(){t||n.apply(u,a),e=null}var u=this,a=arguments;e?clearTimeout(e):t&&n.apply(u,a),e=setTimeout(i,r||1000)}};jQuery.fn[r]=function(n){return n?this.bind("resize",t(n)):this.trigger(r)}}(jQuery,"smartresizelong");

$(document).ready(function() {
	// window.onbeforeunload = function(){
	// 	window.scrollTo(0,0);
	// }
	//
	var authors = ["Patrick Fieger",
		"Tim J. Peters",
		"Anne Pischulski",
		"Volker Matthes",
		"Christian Tielmann"
	]
	var scroll_positions = [];
	var window_scroll_position = 0;
	var actual_index = -1;
	var scroll_speed = 700;
	var devmode = true;
	var height = $(window).height();
	var recognition;

	function c(string) {if(devmode) console.log(string);}
	function a(string) {if(devmode) alert(string);}

	function init(){
		$(".fancybox").fancybox();
		initBGs();
		//if(Modernizr.speechinput) initVoice();

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

		$('.hexa-image').each(function(index, el) {
			$(this).replaceWith('<div class="hexagon hexagon2"><div class="hexagon-in1"><div style="background-image:url('+ $(this).data('image') +')" class="hexagon-in2"></div></div></div>');
		});
	}

	function initMenu(){
		$('section h1').each(function(index, el) {
			var leading_zero = index < 9 ? 0 : "";
			var text = leading_zero + '' + (index+1) + '. ' + $(this).text();

			$('.menu').append('<h2><a href="#'+$(this).closest('section').attr('id')+'">'+ leading_zero + '' +(index+1) + '. ' +  $(this).text()) + '</a></h2>';
			$(this).text(text);

			var d = $(this).data('author');
			var double = d.toString().indexOf('&') > -1 ? true : false;
			var author = "";

			if(d !== "none"){
				if(double){
					var devideand = d.split('&');

					var a1 = authors[parseInt(devideand[0])];
					var a2 = authors[parseInt(devideand[1])];

					author = "- " + a1 + " und " + a2;

				}else{
					var a1 = authors[parseInt(d)];
					author = "- " + a1;
				}

				$(this).attr('data-author',author);
			}


		}).promise().done(function(){
			$('.menu').append('<h2><a href="/prototyp.html">Ausprobieren!</a></h2>')
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
			var top = $('#us').offset().top + 30 + 23;

			$(window).scroll(function() {
				window_scroll_position = $(window).scrollTop();

				for (var j = 0; j < scroll_positions.length; j++) {
					if(window_scroll_position >= scroll_positions[j].offset && window_scroll_position < (scroll_positions[j].offset + scroll_positions[j].height)){
						actual_index = j;
						$('nav a').removeClass('active');
						$('nav a[href="#'+ scroll_positions[j].id +'"]').addClass('active');
					}
				}

				window_scroll_position > top ? $('nav').addClass('sticky') : $('nav').removeClass('sticky');
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

			if($(this).closest('nav').hasClass('show')) $(this).closest('nav').removeClass('show');
		}
	});

	$(document).on('click', '.show_preview', function(event) {
		event.preventDefault();
		$('.preview').css('background-image','url(' + $(this).closest('.trenner').attr('data-bg') + ')').addClass('active');
	});

	$(document).on('click', '.close_preview', function(event) {
		event.preventDefault();
		$('.preview').removeClass('active');
		setTimeout(function() { $('.preview').removeAttr('style') }, 500);
	});

	$(document).on('click touchstart', '#menu_mobile', function(event) {
		event.preventDefault();
		$('nav').toggleClass('show');
	});

	$('#quellen a').each(function(index, el) {
		var text = $(this).text();
		$(this).attr('title', $(this).attr('href'));

		if(text.length > 55) $(this).text(text.substring(0, 55) + '...');
	});

	init();
});