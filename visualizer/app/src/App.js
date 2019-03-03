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
        if (this.props.active)
        {
            return (
                <div className="ECSActiveSystem" onMouseLeave={this.onLeave.bind(this)}>
                    {this.props.system.name}
                </div>
            );
        }
        else
        {
            return (
                <div className="ECSSystem" onMouseEnter={this.onSystemClick.bind(this)}>
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
                <div className="ECSActiveTag" onMouseLeave={this.onLeave.bind(this)}>
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
            activeSystems: [],
            activeViews: [],
            activeGets: [],
            activeHass: []
        }
    }

    onSystemClick(s)
    {
        this.setState({activeSystems: [s], activeViews: s.view, activeGets: s.get, activeHass: s.has})
    }

    onComponentClick(c)
    {
        let state = this.state
        let activeSystems = state.systems.reduce((ass, s) =>
        {
            if (s.view.find(v => v === c)) ass.push(s)
            return ass
        }, [])
        this.setState({activeSystems: activeSystems, activeViews: [c], activeGets: [], activeHass: []})
    }

    onTagClick(e)
    {
    }

    removeActive(e)
    {
        this.setState({activeSystems: [], activeViews: [], activeGets: [], activeHass: []})
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
                        this.state.tags.map(t =>
                        {
                            return <ECSTag tag={t} onLeave={this.removeActive.bind(this)} onTagClick={this.onTagClick.bind(this)} />
                        })
                    }
                </div>
                <div className="Column">
                    {
                        this.state.systems.map(s =>
                        {
                            return <ECSSystem system={s} active={this.state.activeSystems.find(ass => ass === s)} onLeave={this.removeActive.bind(this)} onSystemClick={this.onSystemClick.bind(this)} />
                        })
                    }
                </div>
                <div className="Column">
                    {
                        this.state.components.map(c =>
                        {
                            return <ECSComponent component={c} view={this.state.activeViews.find(ac => ac === c)} get={this.state.activeGets.find(ag => ag === c)} has={this.state.activeHass.find(ah => ah === c)} onLeave={this.removeActive.bind(this)} onComponentClick={this.onComponentClick.bind(this)} />
                        })
                    }
                </div>
            </div>
        );
    }
}

export default App;
