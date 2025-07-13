import React, { useEffect, useState } from 'react';
import { useLocation } from 'react-router-dom';
import { io } from 'socket.io-client';
import './RouteViewer.css'; // ייבוא קובץ העיצוב

const socket = io('http://localhost:3001');

export default function RouteViewer() {
  const location = useLocation();
  const { destination, commands } = location.state || {};

  const [pathPoints, setPathPoints] = useState([]);
  const [distanceTraveled, setDistanceTraveled] = useState(0);
  const [startTime, setStartTime] = useState(null);

  useEffect(() => {
    if (!commands) return;

    const points = calculatePathPoints(commands);
    setPathPoints(points);
    setStartTime(Date.now());

    socket.emit('pathToRobot', commands);

    socket.on('robotStatus', (status) => {
      if (status.total_distance) {
        setDistanceTraveled(status.total_distance);
      }
    });

    return () => socket.off('robotStatus');
  }, [commands]);


  function getVehiclePosition(points, distance) {
    if (points.length < 2) return null;

    let traveled = 0;

    for (let i = 1; i < points.length; i++) {
      const p1 = points[i - 1];
      const p2 = points[i];

      const dx = p2.x - p1.x;
      const dy = p2.y - p1.y;
      const segmentLength = Math.sqrt(dx * dx + dy * dy);

      if (traveled + segmentLength >= distance) {
        const ratio = (distance - traveled) / segmentLength;
        const x = p1.x + dx * ratio;
        const y = p1.y + dy * ratio;
        const angle = Math.atan2(dy, dx) * (180 / Math.PI); // נוסיף את זה!
        return { x, y, angle };
      }

      traveled += segmentLength;
    }

    // אם עברנו את כל המסלול
    const last = points[points.length - 1];
    const dx = last.x - points[points.length - 2].x;
    const dy = last.y - points[points.length - 2].y;
    const angle = Math.atan2(dy, dx) * (180 / Math.PI);
    return { x: last.x, y: last.y, angle };
  }


  function calculatePathPoints(commands) {
    let points = [{ x: 0, y: 0 }];
    let angle = 0;

    commands.forEach(cmd => {
      if (cmd.action === 'forward' && cmd.distance) {
        const rad = (angle * Math.PI) / 180;
        const lastPoint = points[points.length - 1];
        const newX = lastPoint.x + cmd.distance * Math.cos(rad);
        const newY = lastPoint.y + cmd.distance * Math.sin(rad);
        points.push({ x: newX, y: newY });
      } else if (cmd.action === 'right') {
        angle += 90;
      } else if (cmd.action === 'left') {
        angle -= 90;
      }
    });

    return points;
  }

  function normalizePoints(points, size = 400, padding = 20) {
    if (points.length === 0) return [];

    let xs = points.map(p => p.x);
    let ys = points.map(p => p.y);

    let minX = Math.min(...xs);
    let maxX = Math.max(...xs);
    let minY = Math.min(...ys);
    let maxY = Math.max(...ys);

    let scaleX = (size - 2 * padding) / (maxX - minX || 1);
    let scaleY = (size - 2 * padding) / (maxY - minY || 1);
    let scale = Math.min(scaleX, scaleY);

    return points.map(p => ({
      x: padding + (p.x - minX) * scale,
      y: size - (padding + (p.y - minY) * scale),
    }));
  }

  const displayPoints = normalizePoints(pathPoints);
  const totalDistance = commands
    ? commands.filter(c => c.action === 'forward').reduce((sum, c) => sum + (c.distance || 0), 0)
    : 0;
  const remaining = Math.max(0, totalDistance - distanceTraveled);
  const elapsedSeconds = startTime ? Math.floor((Date.now() - startTime) / 1000) : 0;

  const vehiclePos = getVehiclePosition(displayPoints, distanceTraveled);

  return (
    <div className="route-container">
      <h2 className="route-title">מסלול ליעד: {destination}</h2>
      <div className="route-content">
        <svg width={400} height={400} className="route-map">
          {displayPoints.length > 1 && (
            <polyline
              fill="none"
              stroke="blue"
              strokeWidth={3}
              points={displayPoints.map(p => `${p.x},${p.y}`).join(' ')}
            />
          )}

          {displayPoints.map((p, i) => (
            <circle key={i} cx={p.x} cy={p.y} r={6} fill={i === displayPoints.length - 1 ? 'red' : 'green'} />
          ))}

          {vehiclePos && (
            <g transform={`translate(${vehiclePos.x}, ${vehiclePos.y}) rotate(${vehiclePos.angle})`}>
              <circle r={8} fill="orange" />
              <polygon points="0,-6 10,0 0,6" fill="black" /> {/* חץ קטן קדימה */}
            </g>
          )}


        </svg>
        <div className="route-info">
          <p>'מרחק כולל: {Math.round(totalDistance / 100)} מ</p>
          <p>'מרחק שנסע: {Math.round(distanceTraveled / 100)} מ</p>
          <p>'מרחק שנותר: {Math.round(remaining / 100)} מ</p>
          <p>זמן שחלף: {elapsedSeconds} שניות</p>
        </div>

      </div>
    </div>
  );
}
