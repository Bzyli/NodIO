import Node from "./Node.js";
class VolumeNode extends Node {
    createElement() {
      this.params = 100;
      this.name = "volume";

      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      this.element.style.background = '#2ecc71';
      this.element.innerHTML = `Volume`;

      this.element.innerHTML += `<input id="param" type="range" id="volume" name="volume" min="0" max="2" step="0.01" value="0.5"/>`;
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

export default VolumeNode;