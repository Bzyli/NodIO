import Node from "./Nodes/Node.js";
import InputNode from "./Nodes/InputNode.js";
import OutputNode from "./Nodes/OutputNode.js";
import LowCutNode from "./Nodes/LowCutNode.js";
import DelayNode from "./Nodes/DelayNode.js";
import VolumeNode from "./Nodes/VolumeNode.js";
import ReverbNode from "./Nodes/ReverbNode.js";

class NodeEditor {
  constructor(containerId) {
    this.container = document.getElementById(containerId);
    this.nodes = [];
    this.connections = [];
    this.currentConnection = null;
    this.initCanvas();
    this.initPopupMenu();
    this.initEvents();
  }
  initCanvas() {
    this.canvas = document.getElementById('connectionsCanvas');
    this.ctx = this.canvas.getContext('2d');
    this.resizeCanvas();
    window.addEventListener('resize', () => this.resizeCanvas());
  }
  resizeCanvas() {
    this.canvas.width = this.container.clientWidth;
    this.canvas.height = this.container.clientHeight;
    this.drawConnections();
  }
  initPopupMenu() {
    this.popupMenu = document.getElementById('popupMenu');
    // On double-click, show the popup menu at the click position.
    this.container.addEventListener('dblclick', (e) => {
      e.preventDefault();
      this.showPopup(e.clientX, e.clientY);
    });
    // Hide popup when clicking outside.
    document.addEventListener('click', (e) => {
      if (!this.popupMenu.contains(e.target)) {
        this.hidePopup();
      }
    });
    // Create node based on the menu selection.
    this.popupMenu.querySelectorAll('li').forEach(item => {
      item.addEventListener('click', (e) => {
        const type = e.target.getAttribute('data-type');
        this.createNode(type, this.popupMenuX, this.popupMenuY);
        this.hidePopup();
      });
    });
  }
  initEvents() {
    // Update temporary connection line while dragging.
    document.addEventListener('mousemove', (e) => {
      if (this.currentConnection) {
        const rect = this.container.getBoundingClientRect();
        this.currentConnection.tempX = e.clientX - rect.left;
        this.currentConnection.tempY = e.clientY - rect.top;
        this.drawConnections();
      }
    });
    // Cancel connection if mouseup occurs outside a valid input connector.
    document.addEventListener('mouseup', (e) => {
      if (this.currentConnection) {
        this.cancelConnection();
      }
    });
  }
  showPopup(x, y) {
    this.popupMenu.style.left = `${x}px`;
    this.popupMenu.style.top = `${y}px`;
    this.popupMenu.style.display = 'block';
    this.popupMenuX = x;
    this.popupMenuY = y;
  }
  hidePopup() {
    this.popupMenu.style.display = 'none';
  }
  createNode(type, x, y) {
    const id = this.nodes.length + 1;
    let node;
    if (type === 'default') {
      node = new Node(this, id, x, y);
    } else if (type === 'inputOnly') {
      node = new InputNode(this, id, x, y);
    } else if (type === 'outputOnly') {
      node = new OutputNode(this, id, x, y);
    } else if (type === 'lowCut') {
      node = new LowCutNode(this, id, x, y);
    } else if (type === 'delay') {
      node = new DelayNode(this, id, x, y);
    } else if (type === 'volume') {
      node = new VolumeNode(this, id, x, y);
    } else if (type === 'reverb') {
      node = new ReverbNode(this, id, x, y);
    }
    this.nodes.push(node);
  }
  startConnection(fromNode, startX, startY) {
    this.currentConnection = {
      from: fromNode,
      startX: startX,
      startY: startY,
      tempX: startX,
      tempY: startY
    };
  }
  finishConnection(toNode, endX, endY) {
    // Create a permanent connection (ignoring self-connections)
    this.connections.push({
      from: this.currentConnection.from,
      to: toNode
    });
    this.currentConnection = null;
    this.drawConnections();
  }
  cancelConnection() {
    this.currentConnection = null;
    this.drawConnections();
  }
  drawConnections() {
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    // Draw permanent connections.
    this.connections.forEach(conn => {
      const start = conn.from.getOutputConnectorCenter();
      const end = conn.to.getInputConnectorCenter();
      this.drawLine(start.x, start.y, end.x, end.y);
    });
    // Draw the temporary connection if one is in progress.
    if (this.currentConnection) {
      this.drawLine(
        this.currentConnection.startX,
        this.currentConnection.startY,
        this.currentConnection.tempX,
        this.currentConnection.tempY
      );
    }
  }
  drawLine(x1, y1, x2, y2) {
    this.ctx.beginPath();
    this.ctx.moveTo(x1, y1);
    this.ctx.lineTo(x2, y2);
    this.ctx.strokeStyle = 'white';
    this.ctx.lineWidth = 2;
    this.ctx.stroke();
  }

  parseSend(writer) {
    let nodeString = "NODES["
    this.nodes.forEach(node => {
      console.log(node.name, node.params);
      nodeString += node.id + " " + node.name + " " + node.params + ", ";
    });

    nodeString = nodeString.slice(0, -2) + "]"
    console.log(nodeString);
    if(nodeString != "NODE]") {writer.write(new TextEncoder().encode(nodeString));}
    
    let connsString = "CONNS["
    this.connections.forEach(conn => {
      connsString += conn.from.id + " " + conn.to.id + ", ";
    });

    connsString = connsString.slice(0, -2) + "] \n"
    console.log(connsString);
    if (connsString != "CONN]") {writer.write(new TextEncoder().encode(connsString));}
  }
}

var nodeEditor = new NodeEditor("editor");
let writer, reader;
let port;

document.getElementById('connect').addEventListener('click', async () => {
  try {
      port = await navigator.serial.requestPort();
      await port.open({ baudRate: 115200 });

      writer = port.writable.getWriter();
      reader = port.readable.getReader();
      while (true) {
        const { value, done } = await reader.read();
        if (done) {
            console.log("Serial connection closed");
            break;
        }
        console.log("Received: ", new TextDecoder().decode(value));
    }
  } catch (err) {
      console.error("Serial connection error: ", err);
  }
});

document.getElementById('send').addEventListener('click', async () => {
  if (!writer) {
      console.error("Writer not initialized yet!");
      try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 115200 });
  
        writer = port.writable.getWriter();
    } catch (err) {
        console.error("Serial connection error: ", err);
    }
  }
  nodeEditor.parseSend(writer);
});

document.getElementById('clear').addEventListener('click', () => {
  let nodes = document.getElementsByClassName('node');

  while(nodes[0]) {
    nodes[0].parentNode.removeChild(nodes[0]);
  }
  let can = document.getElementById("connectionsCanvas");
  can.getContext("2d").clearRect(0, 0, can.width, can.height);

  nodeEditor.connections = [];
  nodeEditor.nodes = []
});