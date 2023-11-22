import './App.css';
import Menu from './Menu';
import 'semantic-ui-css/semantic.min.css';
import React, { useState, useEffect } from 'react';

function App() {
  // Add this in your component file
  require('react-dom');
  window.React2 = require('react');
  console.log(window.React1 === window.React2);
  return (
    <div className="App">
      <Menu />
    </div>
  );
}

export default App;
