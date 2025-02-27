import Node from "./Node.js";
class ReverbNode extends Node {
    createElement() {
      this.params = [15];
      this.name = "reverb";

      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      this.element.style.height = "120px";
      this.element.style.background = '#2ecc71';
      this.element.innerHTML = `Reverb </br>`;

      this.element.innerHTML += `Roomsize <input id="param" type="number" class="roomsize" name="delay" min="0" max="1" step="0.01" value="0.3" />`;
      this.element.innerHTML += `Damping <input id="param" type="number" class="damping" name="damping" min="0" max="1" step="0.01" value="0.3" /> `
      this.element.querySelector("#param").addEventListener("input", (e) => {
        this.params = [];
        this.params.push(this.element.querySelector(".roomsize").value);
        this.params.push(this.element.querySelector(".damping").value);
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