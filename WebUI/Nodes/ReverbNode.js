import Node from "./Node.js";
class ReverbNode extends Node {
    createElement() {
      this.params = 100;
      this.name = "reverb";

      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      this.element.style.background = '#2ecc71';
      this.element.innerHTML = `Reverb`;

      this.element.innerHTML += `<input id="param" type="number" id="delay" name="delay" min="1" max="1000" step="2" />`;
      this.element.querySelector("#param").addEventListener("input", (e) => {
        this.params = this.element.querySelector("#param").value;
      });
      
      // Create connectors (both)
      this.outputConnector = document.createElement('div');
      this.outputConnector.classList.add('connector', 'output');
      this.element.appendChild(this.outputConnector);
      
      this.inputConnector = document.createElement('div');
      this.inputConnector.classList.add('connector', 'input');
      this.element.appendChild(this.inputConnector);
      
      document.getElementById('editor').appendChild(this.element);
      this.initDrag();
      this.initConnectors();
    }
  }

export default ReverbNode;