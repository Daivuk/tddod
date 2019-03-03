let fs = require('fs')

let components = []
let systems = []
let tags = []

// Read components
{
    let files = fs.readdirSync(`../../src/components`).filter(file => file.endsWith('.h'))
    files.forEach(file => components.push(file.match(/(\w+)\.h/)[1]))
}

// Read systems
{
    let headers = fs.readdirSync(`../../src/systems`).filter(file => file.endsWith('.h'))
    headers.forEach(file => 
    {
        let srcFilename = file.match(/(\w+)\.h/)[1] + ".cpp"
        let systemName = file.match(/(\w+)\.h/)[1]
        let srcContent = fs.readFileSync(`../../src/systems/${srcFilename}`).toString()

        let viewMatch = srcContent.match(/registry\.view\<(.+?)\>/g)
        let views = []
        if (viewMatch)
        {
            views = viewMatch.reduce((v, m) =>
            {
                let view = m.match(/registry\.view\<(.+)\>/)[1]
                let splits = view.replace(/\s/g, '').split(`,`)
                return splits.reduce((v, c) =>
                {
                    if (v.indexOf(c) === -1) return [...v, c]
                    return v
                }, v)
            }, views)
        }

        let hasMatch = srcContent.match(/registry\.has\<(.+?)\>/g)
        let hass = []
        if (hasMatch)
        {
            hass = hasMatch.reduce((v, m) =>
            {
                let view = m.match(/registry\.has\<(.+)\>/)[1]
                let splits = view.replace(/\s/g, '').split(`,`)
                return splits.reduce((v, c) =>
                {
                    if (v.indexOf(c) === -1) return [...v, c]
                    return v
                }, v)
            }, hass)
        }

        let getMatch = srcContent.match(/registry\.get\<(.+?)\>/g)
        let gets = []
        if (getMatch)
        {
            gets = getMatch.reduce((v, m) =>
            {
                let view = m.match(/registry\.get\<(.+)\>/)[1]
                let splits = view.replace(/\s/g, '').split(`,`)
                return splits.reduce((v, c) =>
                {
                    if (v.indexOf(c) === -1) return [...v, c]
                    return v
                }, v)
            }, gets)
        }

        let attacheeMatch = srcContent.match(/registry\.attachee\<Tag\:\:(\w+?)\>/g)
        let attachees = []
        if (attacheeMatch)
        {
            attachees = attacheeMatch.reduce((v, m) =>
            {
                let view = m.match(/registry\.attachee\<Tag\:\:(\w+?)\>/)[1]
                let splits = view.replace(/\s/g, '').split(`,`)
                return splits.reduce((v, c) =>
                {
                    if (v.indexOf(c) === -1) return [...v, c]
                    return v
                }, v)
            }, attachees)
        }

        let system = {
            name: systemName,
            view: views,
            has: hass,
            get: gets,
            attachee: attachees
        }
        systems.push(system)
    })
}

// Read tags
{
    let srcContent = fs.readFileSync(`../../src/tags.h`).toString()
    tags = srcContent.match(/LABEL\((\w+)\)\;/g).map(m => m.match(/LABEL\((\w+)\)\;/)[1])
}

let out = `exports.components = ${JSON.stringify(components)}
exports.systems = ${JSON.stringify(systems)}
exports.tags = ${JSON.stringify(tags)}
`
fs.writeFileSync(`../app/src/registry.js`, out)
