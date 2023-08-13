'use strict'

// set the dimensions and margins of the graph
const margin = { top: 40, right: 60, bottom: 70, left: 60 }
const width = 1366 - margin.left - margin.right
const height = 768 - margin.top - margin.bottom

// append the svg object to the body of the page
const svg = d3.select('#chart')
  .append('svg')
  .attr('width', width + margin.left + margin.right)
  .attr('height', height + margin.top + margin.bottom)
  .append('g')
  .attr('transform', `translate(${margin.left},${margin.top})`)

// create a tooltip
const tooltip = d3.select('#chart')
  .append('div')
  .style('position', 'absolute')
  .style('opacity', 0)
  .attr('class', 'tooltip')
  .style('background-color', 'white')
  .style('border', 'solid')
  .style('border-width', '2px')
  .style('border-radius', '5px')
  .style('padding', '5px')

// Three functions that change the tooltip when user hover / move / leave a bar
function mouseover() {
  tooltip
    .transition()
    .duration(500)
    .style('opacity', 1)
  d3.select(this)
    .style('stroke', '#070fed')
    .style('stroke-width', '3px')
}

function quarterlyMousemove(event, d) {
  tooltip
    .html(`Value : ${d.value} Billion`)
    .style('left', `${event.clientX}px`)
    .style('top', `${event.clientY - 100}px`)
    .style('color', 'black')
    .style('font-size', '14px')
}

function mousemove(event, d) {
  tooltip
    .html(`Value : ${d['Imports of coffee, tea, cocoa & spices from all countries (£billion)']} Billion`)
    .style('left', `${d3.pointer(event, this)[0] + 70}px`)
    .style('top', `${d3.pointer(event, this)[1] + 110}px`)
    .style('color', 'black')
    .style('font-size', '14px')
}

function mouseleave() {
  tooltip
    .style('opacity', 0)
  d3.select(this)
    .style('stroke', 'none')
}

// Parse the Data
d3.csv('data.csv').then(data => {

  // add the options to the button
  d3.select('#yearDropDown')
    .style('display', 'none')

  // A color scale: one color for each group
  let colorScale = undefined

  // Extract categories data
  const annuallyQuarterlySeparatorIndex = data.findIndex(d => d.Date === '' && d['Imports of coffee, tea, cocoa & spices from all countries (£billion)'] === '')
  const annuallyData = data.slice(0, annuallyQuarterlySeparatorIndex)
  const quarterlyMonthlySeperatorIndex = data.findIndex((d, index) => d.Date === '' && d['Imports of coffee, tea, cocoa & spices from all countries (£billion)'] === '' && index > annuallyQuarterlySeparatorIndex)
  const quarterlyData = data.slice(annuallyQuarterlySeparatorIndex + 1, quarterlyMonthlySeperatorIndex)
  const monthlyData = data.slice(quarterlyMonthlySeperatorIndex + 1)
  const firstYear = annuallyData[0].Date

  // Add year label
  svg.append('text')
    .attr('id', 'year')
    .attr('x', width - 64)
    .attr('y', -15)
    .style('font-size', '32px')
    .style('font-weight', '500')

  // Add Y axis
  const y = d3.scaleLinear()
    .domain([0, d3.max(annuallyData.map(d => Number(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])))])
    .range([height, 0])
  const yAxis = d3.axisLeft(y)
  svg.append('g')
    .attr('id', 'y-axis')
    .call(yAxis)
    .style('font-size', '16px')

  // Add Y axis label
  svg.append('text')
    .attr('x', -60)
    .attr('y', -25)
    .text('\u2191 Imports of coffee, tea, cocoa & spices from all countries (£billion)')
    .style('fill', 'white')
    .style('font-size', '18px')

  // X axis
  const x = d3.scaleBand()
    .range([0, width])
    .domain(annuallyData.map(d => d.Date))
    .padding(0.2)
  const xAxis = d3.axisBottom(x)
  svg.append('g')
    .attr('id', 'x-axis')
    .attr('transform', `translate(0, ${height})`)
    .call(xAxis)
    .style('font-size', '16px')

  // gridlines in y axis function
  function make_y_gridlines(grid) {
    return d3.axisLeft(grid)
  }

  svg.append("g")
    .attr("id", "grid1")
    .call(make_y_gridlines(y)
      .tickSize(-width)
      .tickFormat("")
    )

  // Bars
  svg.selectAll('mybar')
    .attr('id', 'mybar')
    .data(annuallyData)
    .join('rect')
    .attr('x', d => x(d.Date))
    .attr('y', () => y(0))
    .attr('width', x.bandwidth())
    .attr('height', () => height - y(0))
    .on('mouseover', mouseover)
    .on('mousemove', mousemove)
    .on('mouseleave', mouseleave)

  showBars(annuallyData)
  let currentPeriod = 'annually'
  let currentType = 'grouped'
  const totalMonthCount = 12
  const groups = new Set(monthlyData.map(d => d.Date.substring(0, 4)))
  let buttonCotrol = 0
  const transformQuarterly = []
  const colorRange = d3.scaleOrdinal()
    .domain(['Q1', 'Q2', 'Q3', 'Q4'])
    .range(["#ff0000", "#00ff00", "#ffff00", "#00d9ff"])
  for (let i = 0; i < quarterlyData.length; i += 4) {
    try {
      let temp = quarterlyData.slice(i, i + 4)
      transformQuarterly.push(
        {
          "Date": temp[0].Date.substring(0, 4),
          "Q1": temp[0]['Imports of coffee, tea, cocoa & spices from all countries (£billion)'],
          "Q2": temp[1]['Imports of coffee, tea, cocoa & spices from all countries (£billion)'],
          "Q3": temp[2]['Imports of coffee, tea, cocoa & spices from all countries (£billion)'],
          "Q4": temp[3]['Imports of coffee, tea, cocoa & spices from all countries (£billion)']
        })
    } catch (err) {
      transformQuarterly.push(
        {
          "Date": quarterlyData[80].Date.substring(0, 4),
          "Q1": quarterlyData[80]['Imports of coffee, tea, cocoa & spices from all countries (£billion)'],
          "Q2": '0',
          "Q3": '0',
          "Q4": '0'
        })
    }
  }
  var subGroupKeys = ["Q1", "Q2", "Q3", "Q4"];

  function showXAxis(data, getDomainItem) {
    // X axis
    const x = d3.scaleBand()
      .range([0, width])
      .domain(data.map(getDomainItem))
      .padding(0.2)
    d3.select('#x-axis').remove()
    svg.append('g')
      .attr('id', 'x-axis')
      .attr('transform', `translate(0, ${height})`)
      .call(d3.axisBottom(x))
      .style('font-size', '16px')
  }

  function updateYAxis(data) {
    // Y axis
    y.domain([0, d3.max(data.map(d => Number(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])))])
    d3.select('#y-axis')
      .transition()
      .duration(2000)
      .call(yAxis)
      .style('font-size', '16px')

    d3.select("#grid1")
      .transition()
      .duration(500)
      .call(make_y_gridlines(y)
        .tickSize(-width)
        .tickFormat("")
      )
  }
  function showBars(data, year) {
    // Animation
    svg.selectAll('rect')
      .data(data)
      .transition()
      .duration(800)
      .attr('y', d => y(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)']))
      .attr('height', d => height - y(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)']))
      .attr('fill', year ? colorScale(year) : '#69b3a2')
      .delay((d, i) => i * 100)
  }
  // A function that update the monthly chart
  function updateMonthlyChart(selectedGroup) {
    // Update year label
    d3.select('#year')
      .text(selectedGroup)

    // Create new data with the selection
    const filteredData = monthlyData.slice((Number(selectedGroup) - firstYear) * totalMonthCount, (Number(selectedGroup) - firstYear + 1) * totalMonthCount)
    if (buttonCotrol == 1) {
      filteredData.sort((left, right) =>
        Number(right['Imports of coffee, tea, cocoa & spices from all countries (£billion)']) -
        Number(left['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])
      )
    } else if (buttonCotrol == 2) {
      filteredData.sort((left, right) =>
        Number(left['Imports of coffee, tea, cocoa & spices from all countries (£billion)']) -
        Number(right['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])
      )
    }
    let index = filteredData.length % totalMonthCount
    if (index !== 0) {
      for (; index !== totalMonthCount; ++index) {
        filteredData.push({
          Date: `${selectedGroup}${['JAN', 'FEB', 'MAR', 'APR', 'MAY', 'JUN', 'JUL', 'AUG', 'SEP', 'OCT', 'NOV', 'DEC'][index]}`,
          'Imports of coffee, tea, cocoa & spices from all countries (£billion)': 0
        })
      }
    }

    showXAxis(filteredData, d => d.Date.substring(4))
    updateYAxis(filteredData)

    // Give these new data to update bars
    showBars(filteredData, selectedGroup)
  }
  function showAnnuallyData() {
    if (document.getElementsByClassName(".legend")) {
      d3.selectAll('.legend').remove();
    }
    if (document.getElementById('groupData')) {
      document.getElementById('groupData').remove()
      d3.selectAll('rect').remove()
    }
    if (document.getElementById('stackedData')) {
      document.getElementById('stackedData').remove()
      d3.selectAll('rect').remove()
    }
    d3.select('#yearDropDown')
      .style('display', 'none')

    d3.select('#year')
      .style('display', 'none')

    const filteredData = [...annuallyData]
    if (buttonCotrol == 1) {
      filteredData.sort((left, right) =>
        Number(right['Imports of coffee, tea, cocoa & spices from all countries (£billion)']) -
        Number(left['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])
      )
    } else if (buttonCotrol == 2) {
      filteredData.sort((left, right) =>
        Number(left['Imports of coffee, tea, cocoa & spices from all countries (£billion)']) -
        Number(right['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])
      )
    }

    // X axis
    x.domain(filteredData.map(d => d.Date))
    d3.select('#x-axis')
      .transition()
      .duration(2000)
      .call(xAxis)

    // Y axis
    y.domain([0, d3.max(filteredData.map(d => Number(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])))])
    d3.select('#y-axis')
      .transition()
      .duration(2000)
      .call(yAxis)
      .style('font-size', '16px')

    d3.select("#grid1")
      .transition()
      .duration(500)
      .call(make_y_gridlines(y)
        .tickSize(-width)
        .tickFormat("")
      )

    // Bars
    const u = svg.selectAll('rect')
      .data(filteredData)

    u.enter()
      .append('rect')
      .merge(u)
      .on('mouseover', mouseover)
      .on('mousemove', mousemove)
      .on('mouseleave', mouseleave)
      .transition()
      .duration(1000)
      .attr('x', d => x(d.Date))
      .attr('y', d => y(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)']))
      .attr('width', x.bandwidth())
      .attr('height', d => height - y(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)']))
      .attr('fill', '#69b3a2')

    u.exit()
      .remove()
  }

  let x1
  var filteredKeys = [];

  function showGroupedQuarterlyData() {
    d3.selectAll('rect').remove()
    d3.select('#yearDropDown')
      .style('display', 'none')
    d3.select('#year')
      .style('display', 'none')
    if (document.getElementById('stackedData')) {
      document.getElementById('stackedData').remove()
    }

    // X-Axis
    x.domain(transformQuarterly.map(d => d.Date))
      .rangeRound([0, width])
      .paddingInner(0.1);
    d3.select('#x-axis')
      .transition()
      .duration(2000)
      .call(xAxis)

    // Y-Axis
    y.domain([0, 1000])
      .rangeRound([height, 0]);
    d3.select('#y-axis')
      .transition()
      .duration(2000)
      .call(yAxis)

    d3.select("#grid1")
      .transition()
      .duration(500)
      .call(make_y_gridlines(y)
        .tickSize(-width)
        .tickFormat("")
      )

    x1 = d3.scaleBand()
      .domain(subGroupKeys)
      .rangeRound([0, x.bandwidth()])
      .padding(0.05);

    var groups = svg.append('g')
      .attr('id', 'groupData')
      .selectAll('g')
      .data(transformQuarterly, d => d.Date)
      .join('g')
      .attr("transform", (d) => "translate(" + x(d.Date) + ",0)");

    groups.selectAll("rect")
      .data(d => subGroupKeys.map(key => ({ key, value: d[key], Date: d.Date })), (d) => "" + d.Date + "_" + d.key)
      .join("rect")
      .on('mouseover', mouseover)
      .on('mousemove', quarterlyMousemove)
      .on('mouseleave', mouseleave)
      .transition()
      .duration(1000)
      .attr("fill", d => colorRange(d.key))
      .attr("x", d => x1(d.key))
      .attr("width", (d) => x1.bandwidth())
      .attr('y', (d) => Math.min(y(0), y(d.value)))
      .attr('height', (d) => Math.abs(y(0) - y(d.value)))

    //boxes to indicate what color for what data
    svg.selectAll(".legend").remove()
    var legend = svg.selectAll(".legend")
      .data(subGroupKeys)
      .enter().append("g")
      .attr("class", "legend")
      .attr("transform", function (d, i) { return "translate(0," + i * 20 + ")"; });
    filteredKeys = []
    //small rectangle/boxes
    const subGroupKeyRange = d3.scaleOrdinal().domain(subGroupKeys).range(subGroupKeys)

    legend.append("rect")
      .attr("class", "small-rect")
      .attr("x", width + 10)
      .attr("width", 20)
      .attr("height", 18)
      .attr('data-key', subGroupKeyRange)
      .style("fill", colorRange)
      .on("click", () => {
        updateGroupedChart(event.target.getAttribute('data-key'))
        if (filteredKeys.length === 0) {
          d3.selectAll(".small-rect").style("fill", colorRange)
        } else if (event.target.getAttribute("style") === "fill: white;") {
          event.target.setAttribute("style", `fill: ${colorRange(event.target.getAttribute('data-key'))};`)
        } else {
          event.target.setAttribute("style", "fill: white;")
        }
      })

    //text to beside rectangle/boxes
    legend.append("text")
      .attr("x", width + 33)
      .attr("y", 9)
      .attr("dy", ".35em")
      .style("fill", "#FFFFFF")
      .text(function (d) { return d; });
  }

  var legend = svg.selectAll(".legend")
    .data(subGroupKeys)
    .enter().append("g")
    .attr("class", "legend")
    .attr("transform", function (d, i) { return "translate(0," + i * 20 + ")"; });

  function updateGroupedChart(key) {
    if (filteredKeys.indexOf(key) == -1) {
      filteredKeys.push(key);
      if (filteredKeys.length == subGroupKeys.length) filteredKeys = [];
    }
    // If selected key is in the list, remove it
    else {
      filteredKeys.splice(filteredKeys.indexOf(key), 1);
    };

    // Create new list of active keys by inverting filteredKeys
    var viewableKeys = [];
    subGroupKeys.forEach(d => {
      if (filteredKeys.indexOf(d) == -1) {
        viewableKeys.push(d);
      }
    });
    console.log(viewableKeys)
    x1 = d3.scaleBand()
      .domain(viewableKeys)
      .rangeRound([0, x.bandwidth()])
      .padding(0.05);

    y.domain([0, d3.max(transformQuarterly, d => d3.max(viewableKeys, k => +d[k]))])
      .rangeRound([height, 0]);
    d3.select('#y-axis')
      .transition()
      .duration(2000)
      .call(yAxis)

    d3.select("#grid1")
      .transition()
      .duration(500)
      .call(make_y_gridlines(y)
        .tickSize(-width)
        .tickFormat("")
      )
    // select all bars on chart
    var bars = svg.selectAll("#groupData").selectAll("rect")

    // If key for bar is in the filtered list remove from chart
    // by setting height and width to 0
    bars.filter(d => filteredKeys.indexOf(d.key) > -1)
      .transition()
      .duration(1000)
      .attr("height", 0)
      .attr("width", 0)
      .attr("y", height);

    // If key for bar is not in the filtered list adjust
    // height and width for data set of viewable bars

    bars.filter(d => filteredKeys.indexOf(d.key) == -1)
      .transition()
      .duration(500)
      .delay(1000)
      .attr("x", d => x1(d.key))
      .attr("width", (d) => x1.bandwidth())
      .transition()
      .duration(800)
      .delay(0)
      .attr('y', (d) => Math.min(y(0), y(d.value)))
      .attr('height', (d) => Math.abs(y(0) - y(d.value)))
      .attr("fill", d => colorRange(d.key))

    // Update legend to indicate which bars are selected/deselected
    // for display by user selection of legend icons

    //d3.select('.legend #smallRectangle').style("fill", 'white')
  }
  function showStackedQuarterlyData() {
    d3.select('#yearDropDown')
      .style('display', 'none')
    d3.select('#year')
      .style('display', 'none')
    d3.selectAll('rect').remove()
    if (document.getElementById('groupData')) {
      document.getElementById('groupData').remove()
    }

    // X-Axis
    x.domain(transformQuarterly.map(d => d.Date))
      .rangeRound([0, width])
      .paddingInner(0.1);
    d3.select('#x-axis')
      .transition()
      .duration(2000)
      .call(xAxis)

    // Y axis
    y.domain([0, d3.max(annuallyData.map(d => Number(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])))])
    d3.select('#y-axis')
      .transition()
      .duration(2000)
      .call(yAxis)
      .style('font-size', '16px')

    d3.select("#grid1")
      .transition()
      .duration(500)
      .call(make_y_gridlines(y)
        .tickSize(-width)
        .tickFormat("")
      )

    const quarterKeys = Object.keys(transformQuarterly[0]).filter(d => d != "Date")
    const stack = d3.stack().keys(quarterKeys)
    svg.append('g')
      .attr('id', 'stackedData')
      .selectAll('.bar')
      .data(stack(transformQuarterly))
      .enter().append('g')
      .attr('class', 'bar')
      .attr('fill', d => colorRange(d.key))
      .selectAll('rect')
      .data(d => d)
      .enter().append('rect')
      .on('mouseover', mouseover)
      .on('mousemove', stackMousemove)
      .on('mouseleave', mouseleave)
      .attr('x', d => x(d.data.Date))
      .attr('y', d => y(d[1]))
      .attr('height', d => y(d[0]) - y(d[1]))
      .attr('width', x.bandwidth())
      .attr('data-value', d => d[1] - d[0])
    //boxes to indicate what color for what data
    svg.selectAll(".legend").remove()

    //small rectangle/boxes
    legend.append("rect")
      .attr("x", width + 10)
      .attr("width", 20)
      .attr("height", 18)
      .style("fill", colorRange);

    //text to beside rectangle/boxes
    legend.append("text")
      .attr("x", width + 33)
      .attr("y", 9)
      .attr("dy", ".35em")
      .style("fill", "#FFFFFF")
      .text(function (d) { return d; });
  }
  function stackMousemove() {
    tooltip
      .html(`Value : ${event.target.getAttribute('data-value')} Billion`)
      .style('left', `${event.clientX}px`)
      .style('top', `${event.clientY - 100}px`)
      .style('color', 'black')
      .style('font-size', '14px')
  }

  function showMonthlyData() {
    if (document.getElementsByClassName(".legend")) {
      d3.selectAll('.legend').remove()
    }
    if (document.getElementById('groupData')) {
      document.getElementById('groupData').remove()
      d3.selectAll('rect').remove()
    }
    if (document.getElementById('stackedData')) {
      document.getElementById('stackedData').remove()
      d3.selectAll('rect').remove()
    }
    d3.selectAll('#select-button option').remove()
    d3.select('#yearDropDown')
      .style('display', 'block')
    d3.select('#select-button')
      .selectAll('myOptions')
      .data(groups)
      .enter()
      .append('option')
      .text(d => d) // text showed in the menu
      .attr('value', d => d) // corresponding value returned by the button

    $("#select-button").val("1998").change();

    colorScale = d3.scaleOrdinal()
      .domain(groups)
      .range(d3.schemeSet2)

    const year = Number(d3.select('#select-button').property('value'))
    d3.select('#year')
      .style('display', 'inline')
      .text(year)
      .style('fill', 'white')

    const filteredData = monthlyData.slice((year - firstYear) * totalMonthCount, (year - firstYear + 1) * totalMonthCount)
    if (buttonCotrol == 1) {
      filteredData.sort((left, right) =>
        Number(right['Imports of coffee, tea, cocoa & spices from all countries (£billion)']) -
        Number(left['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])
      )
    } else if (buttonCotrol == 2) {
      filteredData.sort((left, right) =>
        Number(left['Imports of coffee, tea, cocoa & spices from all countries (£billion)']) -
        Number(right['Imports of coffee, tea, cocoa & spices from all countries (£billion)'])
      )
    }
    // X axis
    x.domain(filteredData.map(d => d.Date.substring(4)))
    d3.select('#x-axis')
      .transition()
      .duration(2000)
      .call(xAxis)
      .style('font-size', '16px')

    // Y axis
    updateYAxis(filteredData)

    // Bars
    const u = svg.selectAll('rect')
      .data(filteredData)

    u.enter()
      .append('rect')
      .merge(u)
      .on('mouseover', mouseover)
      .on('mousemove', mousemove)
      .on('mouseleave', mouseleave)
      .transition()
      .duration(1000)
      .attr('x', d => x(d.Date.substring(4)))
      .attr('y', d => y(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)']))
      .attr('width', x.bandwidth())
      .attr('height', d => height - y(d['Imports of coffee, tea, cocoa & spices from all countries (£billion)']))
      .attr('fill', colorScale(year))

    u.exit()
      .remove()
  }
  // When the button is changed, run the updateChart function
  d3.select('#select-button').on('change', function () {
    updateMonthlyChart(d3.select(this).property('value'))
  })

  d3.select('#action-button').on('click', function () {
    if (buttonCotrol == 0) {
      d3.select(this).html('Sort :Ascending')
      buttonCotrol += 1
    } else if (buttonCotrol == 1) {
      d3.select(this).html('Unsort')
      buttonCotrol += 1
    } else {
      d3.select(this).html('Sort :Descending')
      buttonCotrol = 0
    }
    switch (currentPeriod) {
      case 'annually': showAnnuallyData()
        break
      case 'monthly': updateMonthlyChart(d3.select('#select-button').property('value'))
    }
  })
  $(".period").click(function () {
    const period = this.value
    switch (period) {
      case 'annually':
        showAnnuallyData()
        d3.select('#groupedViewSwitch').style('display', 'none')
        d3.select('#action-button-container').style('display', 'block')
        d3.select('#x-label').text('Year')
        d3.select('#title').text('Year')
        break
      case 'quarterly':
        switch (currentType) {
          case 'grouped':
            showGroupedQuarterlyData()
            break
          case 'stacked':
            showStackedQuarterlyData()
        }
        d3.select('#groupedViewSwitch').style('display', 'block')
        d3.select('#action-button-container').style('display', 'none')
        d3.select('#x-label').text('Quarter')
        d3.select('#title').text('Quarter')
        break
      case 'monthly':
        showMonthlyData()
        d3.select('#groupedViewSwitch').style('display', 'none')
        d3.select('#action-button-container').style('display', 'block')
        d3.select('#x-label').text('Month')
        d3.select('#title').text('Month')
    }
    currentPeriod = period
  });
  $('.type').click(function () {
    const type = this.value
    switch (type) {
      case 'grouped':
        showGroupedQuarterlyData()
        break
      case 'stacked':
        showStackedQuarterlyData()
    }
    currentType = type
  })
})