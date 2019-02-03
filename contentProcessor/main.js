let fs = require('fs')
let PNG = require('pngjs').PNG

let fileData = 
`// Generated file. DO NOT EDIT
#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "components.h"

`

// Generate map
let png = PNG.sync.read(fs.readFileSync("rawAssets/map.png"));
fileData += 
`#define MAP_WIDTH ${png.width}
#define MAP_HEIGHT ${png.height}

`

let path = Array(png.width * png.height)
path.fill(false, 0, png.width * png.height)
let towerSpot = Array(png.width * png.height)
path.fill(false, 0, png.width * png.height)

let startPos = {x:0, y:0}
let endPos = {x:0, y:0}
for (let y = 0; y < png.height; ++y)
{
    for (let x = 0; x < png.width; ++x)
    {
        let k = (y * png.width + x) * 4
        let kk = y * png.width + x
        let r = png.data[k + 0];
        let g = png.data[k + 1];
        let b = png.data[k + 2];

        if (r > 128 && b > 128)
        {
            endPos = {x:x, y:y}
            path[kk] = true
        }
        else if (r > 128)
        {
            startPos = {x:x, y:y}
            path[kk] = true
        }
        else if (g > 128) path[kk] = true
        else if (b > 128) towerSpot[kk] = true
    }
}

function colf(str)
{
    return `${(parseInt(str, 16) / 255).toFixed(6)}f`
}

// Reference: https://stackoverflow.com/questions/5623838/rgb-to-hex-and-hex-to-rgb
function hexToRgba(hex)
{
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex)
    return result 
        ? `${colf(result[1])}, ${colf(result[2])}, ${colf(result[3])}, 1.0f` 
        : `0, 0, 0`
}

// Map vertices
fileData += 
`static const float MAP_VERTS[] = {
`
let pathColor = hexToRgba("#5b6ee1")
for (let y = 0; y < png.height; ++y)
{
    for (let x = 0; x < png.width; ++x)
    {
        let k = y * png.width + x

        if (path[k])
        {
            if (y > 0 && !path[k - png.width])
            {
                fileData += `    ${x}.0f, ${y}.0f, ${pathColor},\n`
                fileData += `    ${x + 1}.0f, ${y}.0f, ${pathColor},\n`
            }
            if (y < png.height - 1 && !path[k + png.width])
            {
                fileData += `    ${x}.0f, ${y + 1}.0f, ${pathColor},\n`
                fileData += `    ${x + 1}.0f, ${y + 1}.0f, ${pathColor},\n`
            }
            if (x > 0 && !path[k - 1])
            {
                fileData += `    ${x}.0f, ${y}.0f, ${pathColor},\n`
                fileData += `    ${x}.0f, ${y + 1}.0f, ${pathColor},\n`
            }
            if (x < png.width - 1 && !path[k + 1])
            {
                fileData += `    ${x + 1}.0f, ${y}.0f, ${pathColor},\n`
                fileData += `    ${x + 1}.0f, ${y + 1}.0f, ${pathColor},\n`
            }
        }
    }
}

// Start and finish arrows
let startColor = hexToRgba("#df7126")
fileData += `    ${startPos.x}.0f, ${startPos.y - 0.3 + 0.5}f, ${startColor},\n`
fileData += `    ${startPos.x}.0f, ${startPos.y + 0.3 + 0.5}f, ${startColor},\n`
fileData += `    ${startPos.x}.0f, ${startPos.y + 0.3 + 0.5}f, ${startColor},\n`
fileData += `    ${startPos.x + 0.3}f, ${startPos.y + 0.5}f, ${startColor},\n`
fileData += `    ${startPos.x + 0.3}f, ${startPos.y + 0.5}f, ${startColor},\n`
fileData += `    ${startPos.x}.0f, ${startPos.y - 0.3 + 0.5}f, ${startColor},\n`

let endColor = hexToRgba("#5fcde4")
fileData += `    ${endPos.x + 0.2 + 0.5}f, ${endPos.y - 0.3 + 0.5}f, ${endColor},\n`
fileData += `    ${endPos.x + 0.2 + 0.5}f, ${endPos.y + 0.3 + 0.5}f, ${endColor},\n`
fileData += `    ${endPos.x + 0.2 + 0.5}f, ${endPos.y + 0.3 + 0.5}f, ${endColor},\n`
fileData += `    ${endPos.x + 0.5 + 0.5}.0f, ${endPos.y + 0.5}f, ${endColor},\n`
fileData += `    ${endPos.x + 0.5 + 0.5}.0f, ${endPos.y + 0.5}f, ${endColor},\n`
fileData += `    ${endPos.x + 0.2 + 0.5}f, ${endPos.y - 0.3 + 0.5}f, ${endColor},\n`

fileData += `};

`

// Nodes
let nodes = [startPos]
while (true)
{
    let node = nodes[nodes.length - 1]
    let k = node.y * png.width + node.x
    if (path[k + 1])
    {
        path[k + 1] = false
        nodes.push({x: node.x + 1, y: node.y})
    }
    else if (path[k - 1])
    {
        path[k - 1] = false
        nodes.push({x: node.x - 1, y: node.y})
    }
    else if (path[k + png.width])
    {
        path[k + png.width] = false
        nodes.push({x: node.x, y: node.y + 1})
    }
    else if (path[k - png.width])
    {
        path[k - png.width] = false
        nodes.push({x: node.x, y: node.y - 1})
    }
    else break
}
nodes.push(endPos)

fileData += `static const Position WAYPOINTS[] = {
    ${nodes.map(node => `{${node.x}.5f, ${node.y}.5f}`).join(`,\n    `)}
};

`

fileData += `#endif
`

fs.writeFileSync("../src/data.h", fileData);
