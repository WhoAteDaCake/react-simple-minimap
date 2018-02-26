import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import NumericInput from '../temp/reason-react-rollup-starter-kit';
import { text } from './text';

import './index.css';

class App extends Component {
  render() {
    return (
      <div className="cont">
        <NumericInput height={150} width={100}>
          <div dangerouslySetInnerHTML={{ __html: text }} />
        </NumericInput>
      </div>
    );
  }
}

ReactDOM.render(<App />, document.querySelector('#app'));
