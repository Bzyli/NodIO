import Node from "./Node.js";
class CompressorNode extends Node {
    createElement() {
      this.params = [40, 5, 3, 2];
      this.name = "compressor";

      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      this.element.style.height = '130px';
      this.element.style.width = '200px';
      this.element.style.background = '#2ecc71';
      this.element.innerHTML = `Compressor </br>`;

      this.element.innerHTML += `Threshold <input id="param" type="range" class="threshold" name="threshold" min="-40" max="0" step="0.1" value="-10"/></br>`;
      this.element.innerHTML += `Ratio <input id="param" type="number" class="ratio" name="ratio" min="1" max="20" step="0.1" value="4"/></br>`;
      this.element.innerHTML += `Attack <input id="param" type="number" class="attack" name="attack" min="0.001" max="0.5" step="0.001" value="0.01"/></br>`;
      this.element.innerHTML += `Release <input id="param" type="number" class="release" name="release" min="0.01" max="0.5" step="0.001" value="0.1"/></br>`;
      this.element.querySelector("#param").addEventListener("input", (e) => {
        this.params = [];
        this.params.push(this.element.querySelector(".threshold").value);
        this.params.push(this.element.querySelector(".ratio").value);
        this.params.push(this.element.querySelector(".attack").value);
        this.params.push(this.element.querySelector(".release").value);
        console.log(this.params);
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

export default CompressorNode;