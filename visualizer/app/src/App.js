import React, { Component } from 'react';
import './App.css';
import Search from './Search';

let registry = require('./registry.js')

class ECSSystem extends Component
{
    onSystemClick(e)
    {
        this.props.onSystemClick(this.props.system)
    }

    onLeave(e)
    {
        this.props.onLeave(this.props.system)
    }

    render()
    {
        let has = this.props.has ? " ECSHasComponent" : ""
        if (this.props.view)
        {
            return (
                <div className={`ECSActiveSystem${has}`} onMouseLeave={this.onLeave.bind(this)}>
                    {this.props.system.name}
                </div>
            );
        }
        else if (this.props.get)
        {
            return (
                <div className={`ECSGetComponent${has}`} onMouseLeave={this.onLeave.bind(this)}>
                    {this.props.system.name}
                </div>
            );
        }
        else
        {
            return (
                <div className={`ECSSystem${has}`} onMouseEnter={this.onSystemClick.bind(this)}>
                    {this.props.system.name}
                </div>
            );
        }
    }
}

class ECSTag extends Component
{
    onTagClick(e)
    {
        this.props.onTagClick(this.props.tag)
    }

    onLeave(e)
    {
        this.props.onLeave(this.props.tag)
    }

    render()
    {
        if (this.props.active)
        {
            return (
                <div className="ECSActiveSystem" onMouseLeave={this.onLeave.bind(this)}>
                    {this.props.tag}
                </div>
            );
        }
        else
        {
            return (
                <div className="ECSTag" onMouseEnter={this.onTagClick.bind(this)}>
                    {this.props.tag}
                </div>
            );
        }
    }
}

class ECSComponent extends Component
{
    onComponentClick(e)
    {
        this.props.onComponentClick(this.props.component)
    }

    onLeave(e)
    {
        this.props.onLeave(this.props.system)
    }

    render()
    {
        let has = this.props.has ? " ECSHasComponent" : ""
        if (this.props.view)
        {
            return (
                <div className={`ECSViewComponent${has}`} onMouseLeave={this.onLeave.bind(this)}>
                    {this.props.component}
                </div>
            );
        }
        else if (this.props.get)
        {
            return (
                <div className={`ECSGetComponent${has}`} onMouseLeave={this.onLeave.bind(this)}>
                    {this.props.component}
                </div>
            );
        }
        else
        {
            return (
                <div className={`ECSComponent${has}`} onMouseEnter={this.onComponentClick.bind(this)}>
                    {this.props.component}
                </div>
            );
        }
    }
}

class App extends Component
{
    constructor()
    {
        super()
        this.state = {
            systems: registry.systems,
            components: registry.components,
            tags: registry.tags,
            activeSystemViews: [],
            activeSystemGets: [],
            activeSystemHass: [],
            activeTags: [],
            activeViews: [],
            activeGets: [],
            activeHass: []
        }
    }

    onSystemClick(s)
    {
        this.setState({
            activeSystemViews: [s], 
            activeSystemGets: [], 
            activeSystemHass: [],
            activeTags: s.attachee,
            activeViews: s.view, 
            activeGets: s.get, 
            activeHass: s.has
        })
    }

    onComponentClick(c)
    {
        let activeSystemViews = registry.systems.reduce((ass, s) =>
        {
            if (s.view.find(v => v === c)) ass.push(s)
            return ass
        }, [])

        let activeSystemGets = registry.systems.reduce((ass, s) =>
        {
            if (s.get.find(v => v === c)) ass.push(s)
            return ass
        }, [])

        let activeSystemHass = registry.systems.reduce((ass, s) =>
        {
            if (s.has.find(v => v === c)) ass.push(s)
            return ass
        }, [])

        this.setState({
            activeSystemViews: activeSystemViews, 
            activeSystemGets: activeSystemGets,
            activeSystemHass: activeSystemHass,
            activeTags: [],
            activeViews: [c], 
            activeGets: [], 
            activeHass: []
        })
    }

    onTagClick(t)
    {
        let attachees = registry.systems.reduce((ass, s) =>
        {
            if (s.attachee.find(v => v === t)) ass.push(s)
            return ass
        }, [])

        this.setState({
            activeSystemViews: attachees, 
            activeSystemGets: [],
            activeSystemHass: [],
            activeTags: [t],
            activeViews: [], 
            activeGets: [], 
            activeHass: []
        })
    }

    removeActive(e)
    {
        this.setState({
            activeSystemViews: [], 
            activeSystemGets: [],
            activeSystemHass: [],
            activeTags: [],
            activeViews: [], 
            activeGets: [], 
            activeHass: []
        })
    }

    onClearSearch()
    {
        this.setState({
            systems: registry.systems,
            components: registry.components,
            tags: registry.tags
        })
    }

    onSearch(text, unlock)
    {
        text = text.toUpperCase()

        let systems = registry.systems.filter(s => s.name.toUpperCase().includes(text))
        let components = registry.components.filter(c => c.toUpperCase().includes(text))
        let tags = registry.tags.filter(t => t.toUpperCase().includes(text))

        this.setState({
            systems: systems,
            components: components,
            tags: tags
        })  

        unlock()
    }

    render()
    {
        let systems = this.state.systems.concat(this.state.activeSystemViews.filter(item => this.state.systems.indexOf(item) < 0))
        systems = systems.concat(this.state.activeSystemGets.filter(item => systems.indexOf(item) < 0))
        systems = systems.concat(this.state.activeSystemHass.filter(item => systems.indexOf(item) < 0))

        let components = this.state.components.concat(this.state.activeViews.filter(item => this.state.components.indexOf(item) < 0))
        components = components.concat(this.state.activeGets.filter(item => components.indexOf(item) < 0))
        components = components.concat(this.state.activeHass.filter(item => components.indexOf(item) < 0))

        let tags = this.state.tags.concat(this.state.activeTags.filter(item => this.state.tags.indexOf(item) < 0))

        return (
            <div className="App">
                <div className="Row">
                    <Search ref="Search"
                            placeholder="search keyword"
                            minCharCount="1"
                            onClearSearch={this.onClearSearch.bind(this)} 
                            onSearch={this.onSearch.bind(this)}
                            autofocus={false} />
                </div>
                <div className="Column">
                    {
                        tags.map(t =>
                        {
                            return <ECSTag tag={t} 
                                active={this.state.activeTags.find(ass => ass === t)} 
                                onLeave={this.removeActive.bind(this)} 
                                onTagClick={this.onTagClick.bind(this)} />
                        })
                    }
                </div>
                <div className="Column">
                    {
                        systems.map(s =>
                        {
                            return <ECSSystem system={s} 
                                view={this.state.activeSystemViews.find(ass => ass === s)} 
                                get={this.state.activeSystemGets.find(ag => ag === s)} 
                                has={this.state.activeSystemHass.find(ah => ah === s)} 
                                onLeave={this.removeActive.bind(this)} 
                                onSystemClick={this.onSystemClick.bind(this)} />
                        })
                    }
                </div>
                <div className="Column">
                    {
                        components.map(c =>
                        {
                            return <ECSComponent component={c} 
                                view={this.state.activeViews.find(ac => ac === c)} 
                                get={this.state.activeGets.find(ag => ag === c)} 
                                has={this.state.activeHass.find(ah => ah === c)} 
                                onLeave={this.removeActive.bind(this)} 
                                onComponentClick={this.onComponentClick.bind(this)} />
                        })
                    }
                </div>
            </div>
        );
    }
}

export default App;
