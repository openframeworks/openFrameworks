/**
 * SyntaxHighlighter
 * http://alexgorbatchev.com/SyntaxHighlighter
 *
 * SyntaxHighlighter is donationware. If you are using it, please donate.
 * http://alexgorbatchev.com/SyntaxHighlighter/donate.html
 *
 * @version
 * 3.0.83 (Sun, 11 Aug 2013 09:19:22 GMT)
 *
 * @copyright
 * Copyright (C) 2004-2013 Alex Gorbatchev.
 *
 * @license
 * Dual licensed under the MIT and GPL licenses.
 */
var dp = {
	SyntaxHighlighter : {}
};

dp.SyntaxHighlighter = {
	parseParams: function(
						input,
						showGutter, 
						showControls, 
						collapseAll, 
						firstLine, 
						showColumns
						)
	{
		function getValue(list, name)
		{
			var regex = XRegExp('^' + name + '\\[(?<value>\\w+)\\]$', 'gi'),
				match = null
				;
			
			for (var i = 0; i < list.length; i++) 
				if ((match = XRegExp.exec(list[i], regex)) != null)
					return match.value;
			
			return null;
		};
		
		function defaultValue(value, def)
		{
			return value != null ? value : def;
		};
		
		function asString(value)
		{
			return value != null ? value.toString() : null;
		};

		var parts = input.split(':'),
			brushName = parts[0],
			options = {},
			straight = { 'true' : true }
			reverse = { 'true' : false },
			result = null,
			defaults = SyntaxHighlighter.defaults
			;
		
		for (var i in parts)
			options[parts[i]] = 'true';

		showGutter = asString(defaultValue(showGutter, defaults.gutter));
		showControls = asString(defaultValue(showControls, defaults.toolbar));
		collapseAll = asString(defaultValue(collapseAll, defaults.collapse)); 
		showColumns = asString(defaultValue(showColumns, defaults.ruler));
		firstLine = asString(defaultValue(firstLine, defaults['first-line'])); 

		return {
			brush			: brushName,
			gutter			: defaultValue(reverse[options.nogutter], showGutter),
			toolbar			: defaultValue(reverse[options.nocontrols], showControls),
			collapse		: defaultValue(straight[options.collapse], collapseAll),
			// ruler			: defaultValue(straight[options.showcolumns], showColumns),
			'first-line'	: defaultValue(getValue(parts, 'firstline'), firstLine)
		};
	},
	
	HighlightAll: function(
						name, 
						showGutter /* optional */, 
						showControls /* optional */, 
						collapseAll /* optional */, 
						firstLine /* optional */, 
						showColumns /* optional */
						)
	{
		function findValue()
		{
			var a = arguments;
			
			for (var i = 0; i < a.length; i++) 
			{
				if (a[i] === null) 
					continue;
				
				if (typeof(a[i]) == 'string' && a[i] != '') 
					return a[i] + '';
				
				if (typeof(a[i]) == 'object' && a[i].value != '') 
					return a[i].value + '';
			}
			
			return null;
		};

		function findTagsByName(list, name, tagName)
		{
			var tags = document.getElementsByTagName(tagName);
			
			for (var i = 0; i < tags.length; i++) 
				if (tags[i].getAttribute('name') == name) 
					list.push(tags[i]);
		}
		
		var elements = [],
			highlighter = null,
			registered = {},
			propertyName = 'innerHTML'
			;
		
		// for some reason IE doesn't find <pre/> by name, however it does see them just fine by tag name... 
		findTagsByName(elements, name, 'pre');
		findTagsByName(elements, name, 'textarea');

		if (elements.length === 0)
			return;
		
		for (var i = 0; i < elements.length; i++)
		{
			var element = elements[i],
				params = findValue(
					element.attributes['class'], element.className, 
					element.attributes['language'], element.language
					),
				language = ''
				;
			
			if (params === null) 
				continue;

			params = dp.SyntaxHighlighter.parseParams(
				params,
				showGutter, 
				showControls, 
				collapseAll, 
				firstLine, 
				showColumns
				);

			SyntaxHighlighter.highlight(params, element);
		}
	}
};
