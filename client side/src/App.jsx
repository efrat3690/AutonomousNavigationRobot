// App.jsx
import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import DestinationSelector from './components/DestinationSelector';
import RouteViewer from './components/RouteViewer';

function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<DestinationSelector />} />
        <Route path="/route" element={<RouteViewer />} />
      </Routes>
    </Router>
  );
}

export default App;
