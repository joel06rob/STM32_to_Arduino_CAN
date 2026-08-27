//Setup websocket (Connection for arduino)
const socket = io(`http://${window.location.host}`);

//Wait for page elements to load
document.addEventListener('DOMContentLoaded', () =>{
  
  //Get UI elements (CAN Frame)
  const sofElement = document.getElementById("sof");
  const idElement = document.getElementById("canid");
  const rtrElement = document.getElementById("rtr");
  const ideElement = document.getElementById("ide");
  const rElement = document.getElementById("r");
  const dlcElement = document.getElementById("dlc");
  const dataElement = document.getElementById("data");
  const canvas = document.getElementById("canWaveform");
  const ctx = canvas.getContext("2d");


  //Draw the CAN Signsl Diagram
  function drawCANWaveform(bits)
  {

      //Clear previous
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      
      
      const bitWidth = 15; //px Length of bit on diagram
      const canHDominant = 50; //px height of CANH
      const recessive = 100; //px heigh of middle
      const canLDominant = 150; //px height of CANL
  
      let x = 50;

      //Labels for diagram
      ctx.font = "16px Arial";
      ctx.fillText("CANH", 5, canHDominant);
      ctx.fillText("CANL", 5, canLDominant);
  
      //Work out starting positions
      let previousHY;
      let previousLY; 

      if(bits[0] === "0"){

        //First bit is dominant (H L)
        previousLY = canLDominant;
        previousHY = canHDominant;
        
      }
      else{

        //First bit is recessive (-)
        previousHY = recessive;
        previousLY = recessive;

      }
    
      //Initialise start CANH line
      ctx.beginPath();
      ctx.moveTo(x, previousHY);
  
      for (let i = 0; i < bits.length; i++)
      {
          const bit = bits[i];
  
          let canHY;
  
          if (bit === "0")
          {
              canHY = canHDominant;
          }
          else
          {
              canHY = recessive;
          }
  
          //Draw vertical transition
          ctx.lineTo(x, canHY);
  
          //Draw horizontal part
          ctx.lineTo(x + bitWidth, canHY);

          //Labels
          ctx.fillText(bit, x + bitWidth / 2, 190);
  
          x += bitWidth;
          previousHY = canHY;
      }
  
      ctx.stroke();
  
  
      //---------- CANL ----------
  
      x = 50;
  
      ctx.beginPath();
      ctx.moveTo(x, previousLY);
  
      for (let i = 0; i < bits.length; i++)
      {
          const bit = bits[i];
  
          let canLY;
  
          if (bit === "0")
          {
              canLY = canLDominant;
          }
          else
          {
              canLY = recessive;
          }
  
          //Vertical transition
          ctx.lineTo(x, canLY);
  
          //Horizontal part
          ctx.lineTo(x + bitWidth, canLY);

          //Labels
          ctx.fillText(bit, x + bitWidth / 2, 190);
  
          x += bitWidth;
          previousLY = canLY;
      }
  
      ctx.stroke();
  }
  
  
  
  //Python -> JS Data
  socket.on("binary_data", (frame) => {
    sofElement.textContent = frame.sof;
    idElement.textContent = frame.canid;
    rtrElement.textContent = frame.rtr;
    ideElement.textContent = frame.ide;
    rElement.textContent = frame.r;
    dlcElement.textContent = frame.dlc;
    dataElement.textContent = frame.datastring;

    const frameBits =
        frame.sof +
        frame.canid +
        frame.rtr +
        frame.ide +
        frame.r +
        frame.dlc +
        frame.datastring;

    drawCANWaveform(frameBits);
  });

  
})