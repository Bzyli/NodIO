import Node from "./Node.js";
// Node with only an output connector
class InputNode extends Node {
    createElement() {
      this.params = [1];
      this.name = "in";

      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      this.element.style.background = '#f39c12';
      this.element.innerHTML = `Audio In`;

      this.element.innerHTML +=  `<select id="param" name="in"><option value="1">In 1</option><option value="2">In 2</option></select>`;
      this.element.querySelector("#param").addEventListener("input", (e) => {
        this.params = []
        this.params.push(this.element.querySelector("#param").value);
      });

      // Only create the output connector (right side)
      this.outputConnector = document.createElement('div');
      this.outputConnector.classList.add('connector', 'output');
      this.element.appendChild(this.outputConnector);
      
      document.getElementById('editor').appendChild(this.element);
      this.initDrag();
      this.initConnectors();
    }
    
    initConnectors() {
      // Only set up event for output connector
      this.outputConnector.addEventListener('mousedown', (e) => {
        e.stopPropagation();
        const rect = this.outputConnector.getBoundingClientRect();
        const editorRect = this.editor.container.getBoundingClientRect();
        const startX = rect.left - editorRect.left + rect.width / 2;
        const startY = rect.top - editorRect.top + rect.height / 2;
        this.editor.startConnection(this, startX, startY);
      });
    }
    getOutputConnectorCenter() {
      const rect = this.outputConnector.getBoundingClientRect();
      const editorRect = this.editor.container.getBoundingClientRect();
      return {
        x: rect.left - editorRect.left + rect.width / 2,
        y: rect.top - editorRect.top + rect.height / 2
      };
    }
  }

export default InputNode;