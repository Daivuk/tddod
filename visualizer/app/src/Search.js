import React, { Component } from 'react';

// Props:
//  placeholder
//  minCharCount
//  onClearSearch()
//  onSearch(text)
class Search extends Component
{
    constructor()
    {
        super();

        this.locked = false;
        this.searchText = "";
        this.currentText = "";
    }

    onTextChanged()
    {
        this.currentText = this.refs.txtName.value;
        if (!this.locked)
        {
            this.updateSearch();
        }
    }

    updateSearch()
    {
        this.searchText = this.currentText;
        if (this.searchText.length < (this.props.minCharCount ? this.props.minCharCount : 1))
        {
            this.props.onClearSearch();
        }
        else
        {
            this.locked = true;
            this.props.onSearch(this.searchText, this.unlock.bind(this));
        }
    }

    componentDidMount()
    {
        this.refs.txtName.value = "";
        this.searchText = "";
        this.currentText = "";
        this.locked = false;

        if (this.props.autofocus)
        {
            this.refs.txtName.focus();
        }
    }

    unlock()
    {
        this.locked = false;
        if (this.searchText !== this.currentText)
        {
            this.updateSearch();
        }
    }

    render()
    {
        return (
            <input className="Search" type="text" ref="txtName" placeholder={this.props.placeholder}
                        onChange={this.onTextChanged.bind(this)} />
        );
    }
}

export default Search;
