import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import NumericInput from '../temp/reason-react-rollup-starter-kit';

class App extends Component {
  render() {
    return (
      <div>
        <NumericInput age="asdasdas" />
      </div>
    );
  }
}

ReactDOM.render(<App />, document.querySelector('#app'));
