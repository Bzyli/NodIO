import Node from "./Nodes/Node.js";
import TextNode from "./Nodes/TextNode.js";
import ImageNode from "./Nodes/ImageNode.js";
import InputNode from "./Nodes/InputNode.js";
import OutputNode from "./Nodes/OutputNode.js";

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
    } else if (type === 'text') {
      node = new TextNode(this, id, x, y);
    } else if (type === 'image') {
      node = new ImageNode(this, id, x, y);
    } else if (type === 'inputOnly') {
      node = new InputNode(this, id, x, y);
    } else if (type === 'outputOnly') {
      node = new OutputNode(this, id, x, y);
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
}

new NodeEditor("editor");