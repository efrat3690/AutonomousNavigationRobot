import React, { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import './DestinationSelector.css'; // ייבוא קובץ העיצוב

export default function DestinationSelector() {
  const [paths, setPaths] = useState({});
  const navigate = useNavigate();

  useEffect(() => {
    fetch('/paths.json')
      .then(res => res.json())
      .then(data => setPaths(data));
  }, []);

 useEffect(() => {
  fetch('http://localhost:3001/paths')  // ← נכון! שרת ה-Node שלך
    .then(res => res.json())
    .then(data => setPaths(data));
}, []);


  const handleDestinationClick = (dest) => {
    const commands = paths[dest];
    if (commands && Array.isArray(commands)) {
      navigate('/route', { state: { destination: dest, commands } });
    }
  };

  return (
    <div className="destination-container">
      <h1 className="destination-title">ברוכים הבאים לנסיעה אוטונומית בטוחה</h1>
      <p className="destination-subtitle">לחצו על היעד שלכם וניקח אתכם בקלות</p>

      <div className="destination-buttons">
        {Object.keys(paths).map(dest => (
          <button
            key={dest}
            onClick={() => handleDestinationClick(dest)}
            className="destination-button"
          >
            {dest}
          </button>
        ))}
      </div>
    </div>
  );
}
