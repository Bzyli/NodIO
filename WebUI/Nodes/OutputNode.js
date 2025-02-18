import Node from "./Node.js";
// Node with only an input connector
class OutputNode extends Node {
    createElement() {
      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      // Different background to distinguish it
      this.element.style.background = '#9b59b6';
      this.element.innerHTML = `Input Only Node ${this.id}`;
      
      // Only create the input connector (left side)
      this.inputConnector = document.createElement('div');
      this.inputConnector.classList.add('connector', 'input');
      this.element.appendChild(this.inputConnector);
      
      document.getElementById('editor').appendChild(this.element);
      this.initDrag();
      this.initConnectors();
    }

    initConnectors() {
      // Only set up event for input connector
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
    
    getInputConnectorCenter() {
      const rect = this.inputConnector.getBoundingClientRect();
      const editorRect = this.editor.container.getBoundingClientRect();
      return {
        x: rect.left - editorRect.left + rect.width / 2,
        y: rect.top - editorRect.top + rect.height / 2
      };
    }
  }

export default OutputNode;
  