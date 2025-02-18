class Node {
    constructor(editor, id, x, y) {
      this.editor = editor;
      this.id = id;
      this.x = x;
      this.y = y;
      this.createElement();
    }

    createElement() {
      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      // Default styling for nodes with both connectors
      this.element.style.background = '#3498db';
      this.element.innerHTML = `Node ${this.id}`;
      
      // Create output connector (right side)
      this.outputConnector = document.createElement('div');
      this.outputConnector.classList.add('connector', 'output');
      this.element.appendChild(this.outputConnector);
      
      // Create input connector (left side)
      this.inputConnector = document.createElement('div');
      this.inputConnector.classList.add('connector', 'input');
      this.element.appendChild(this.inputConnector);
      
      document.getElementById('editor').appendChild(this.element);
      this.initDrag();
      this.initConnectors();
    }

    initDrag() {
      let offsetX, offsetY;
      this.element.addEventListener('mousedown', (e) => {
        // Do not initiate drag when clicking on a connector
        if (e.target.classList.contains('connector')) return;
        offsetX = e.clientX - this.element.offsetLeft;
        offsetY = e.clientY - this.element.offsetTop;
        const onMouseMove = (e) => {
          this.x = e.clientX - offsetX;
          this.y = e.clientY - offsetY;
          this.element.style.left = `${this.x}px`;
          this.element.style.top = `${this.y}px`;
          // Update connections as the node moves
          this.editor.drawConnections();
        };
        const onMouseUp = () => {
          document.removeEventListener('mousemove', onMouseMove);
          document.removeEventListener('mouseup', onMouseUp);
        };
        document.addEventListener('mousemove', onMouseMove);
        document.addEventListener('mouseup', onMouseUp);
      });
    }

    initConnectors() {
      // When pressing on the output connector, start a connection
      this.outputConnector.addEventListener('mousedown', (e) => {
        e.stopPropagation();
        const rect = this.outputConnector.getBoundingClientRect();
        const editorRect = this.editor.container.getBoundingClientRect();
        const startX = rect.left - editorRect.left + rect.width / 2;
        const startY = rect.top - editorRect.top + rect.height / 2;
        this.editor.startConnection(this, startX, startY);
      });
      // When releasing on the input connector, complete the connection
      this.inputConnector.addEventListener('mouseup', (e) => {
        e.stopPropagation();
        if (this.editor.currentConnection && this.editor.currentConnection.from !== this) {
          const rect = this.inputConnector.getBoundingClientRect();
          const editorRect = this.editor.container.getBoundingClientRect();
          const endX = rect.left - editorRect.left + rect.width / 2;
          const endY = rect.top - editorRect.top + rect.height / 2;
          this.editor.finishConnection(this, endX, endY);
        }
      });
    }

    // Helper method to get connector centers relative to the editor
    getOutputConnectorCenter() {
      const rect = this.outputConnector.getBoundingClientRect();
      const editorRect = this.editor.container.getBoundingClientRect();
      return {
        x: rect.left - editorRect.left + rect.width / 2,
        y: rect.top - editorRect.top + rect.height / 2
      };
    }
    getInputConnectorCenter() {
      const rect = this.inputConnector.getBoundingClientRect();
      const editorRect = this.editor.container.getBoundingClientRect();
      return {
        x: rect.left - editorRect.left + rect.width / 2,
        y: rect.top - editorRect.top + rect.height / 2
      };
    }
  }

export default Node;