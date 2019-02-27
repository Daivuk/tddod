let fs = require('fs')
let PNG = require('pngjs').PNG

let fileData = 
`/*
    Generated file. DO NOT EDIT
    Here lies all resources game data from raw assets.
*/
#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "components/Position.h"
#include "components/TexCoord.h"
#include "components/Wave.h"
#include "helpers/CritterHelpers.h"

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

static const int WAYPOINT_COUNT = ${nodes.length};

`

fileData += `static const Position AIR_WAYPOINTS[] = {
    {${startPos.x}.5f, ${startPos.y}.5f},
    {${endPos.x}.5f, ${endPos.y}.5f}
};

static const int AIR_WAYPOINT_COUNT = 2;

`

// Generate waves
let waves = []
png = PNG.sync.read(fs.readFileSync("rawAssets/waves.png"));
for (let y = 0; y < png.height; ++y)
{
    let x = 0
    let wave = []
    for (; x < png.width; ++x)
    {
        let k = (y * png.width + x) * 4
        let r = png.data[k + 0];
        let g = png.data[k + 1];
        let b = png.data[k + 2];

        if (x == 0 && r == 0 && g == 0 && b == 0) break

        if (r == 255 && g == 0 && b == 255)
        {
            wave.push({
                fn: "Critters::createFlyer",
                time: x / 4
            })
        }
        if (r == 255 && g == 255 && b == 0)
        {
            wave.push({
                fn: "Critters::createBehemoth",
                time: x / 4
            })
        }
        else if (b == 255)
        {
            wave.push({
                fn: "Critters::createPeasant",
                time: x / 4
            })
        }
        else if (g == 255)
        {
            wave.push({
                fn: "Critters::createWarrior",
                time: x / 4
            })
        }
        else if (r == 255)
        {
            wave.push({
                fn: "Critters::createThief",
                time: x / 4
            })
        }
    }
    if (wave.length == 0) break
    wave.push({
        fn: "nullptr",
        time: wave[wave.length - 1].time
    })
    waves.push(wave)
}

for (let i = 0; i < waves.length; ++i)
{
    let wave = waves[i]
    fileData += `static const float WAVE_${i}_TIMES[] = {
    ${wave.map(spawn => `${(spawn.time).toFixed(2)}f`).join(`, `)}
};
`
    fileData += `static const CREATE_CRITTER_FN WAVE_${i}_FACTORIES[] = {
    ${wave.map(spawn => `${spawn.fn}`).join(`, `)}
};
`
}

fileData += `static const Wave WAVES[] = {
    ${waves.map((wave, i) => `{${wave.length}, WAVE_${i}_TIMES, WAVE_${i}_FACTORIES}`).join(`,\n    `)}
};

static const int WAVE_COUNT = ${waves.length};
static const Position WAVE_START_POSITION = {${startPos.x + .5}f, ${startPos.y + .5}f};

`

// Font
png = PNG.sync.read(fs.readFileSync("../assets/font.png"));
fileData += `static const TexCoord FONT[][2] = {
`
let base = '!'.charCodeAt(0)
let len = '~'.charCodeAt(0)
let v = 20 / png.height
for (let i = base; i < len; ++i)
{
    let c = i - base
    fileData += `    {{(float)${(c * 20) / png.width}, 0.0f}, {(float)${((c + 1) * 20) / png.width}, (float)${v}}},\n`
}
fileData += `};
static const int FIRST_CHAR = '!';
static const int LAST_CHAR = '~';
static const TexCoord WHITE_UV = {(float)${10 / png.width}, (float)${26 / png.height}};

`

function fileToConstantString(filename)
{
    let constName = filename.replace('.', '_').toUpperCase()
    let content = fs.readFileSync(`rawAssets/${filename}`).toString().replace(/(?:\r\n|\r|\n)/g, '')
    fileData += `static const char *${constName} = "${content}";
`
}

// Shaders
fs.readdirSync("rawAssets/").filter(file => file.endsWith('.vert') || file.endsWith('.frag')).forEach(file => fileToConstantString(file))
fileData += `
`

fileData += `#endif
`

fs.writeFileSync("../src/data.h", fileData);
