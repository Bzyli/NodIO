import Node from "./Node.js";
class ParamEqNode extends Node {
    createElement() {
      this.params = [40, 5, 3, 2];
      this.name = "eq";

      this.element = document.createElement('div');
      this.element.classList.add('node');
      this.element.style.left = `${this.x}px`;
      this.element.style.top = `${this.y}px`;
      this.element.style.height = '130px';
      this.element.style.width = '200px';
      this.element.style.background = '#2ecc71';
      this.element.innerHTML = `Parametric eq </br>`;

      this.element.innerHTML += `Frequency <input id="param" type="number" class="freq" name="freq" min="20" max="20000" step="5" value="100"/></br>`;
      this.element.innerHTML += `Gain <input id="param" type="number" class="gain" name="gain" min="-10" max="10" step="0.1" value="3"/></br>`;
      this.element.innerHTML += `Q factor <input id="param" type="number" class="q" name="q" min="0.01" max="100" step="0.01" value="1"/></br>`;
      this.element.querySelector("#param").addEventListener("input", (e) => {
        this.params = [];
        this.params.push(this.element.querySelector(".freq").value);
        this.params.push(this.element.querySelector(".gain").value);
        this.params.push(this.element.querySelector(".q").value);
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

export default ParamEqNode;