// Mineral max refers to the starting mineral level
const MINERAL_MAX = 100.0
const MINERAL_MIN = 0.0

const WATER_MAX = 45.0
const WATER_MIN = 0.0

const TEMPERATURE_MAX = 65.0
const TEMPERATURE_MIN = 45.0


document.addEventListener('DOMContentLoaded', () => {
    // Initial fetch when the page loads
    fetchData();
  
    // Fetch data every 2 seconds
    setInterval(fetchData, 1000);
  });
  
  // Fetch data from server
  async function fetchData() {
    try {
      const response = await fetch('/getData');
      const data = await response.text();
      updateStatusBars(data);
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  }
  
  function updateStatusBars(data) {
    data = data.split("\n");
    var mineral_value = data[0];
    var temp_value = data[1];
    var water_value = data[2];

	  console.log(mineral_value);

    var mineral_Bar = document.getElementById('mnrl_adj');  
    var mineral_text = document.getElementById('mnrl_value');  

    mineral_text.innerHTML = (mineral_value) + "%";
    mineral_Bar.style.height = Math.ceil(100*(1 - (mineral_value - MINERAL_MIN) / (MINERAL_MAX - MINERAL_MIN))) + "%";  

    var water_Bar = document.getElementById('watr_adj');  
    var water_text = document.getElementById('watr_value'); 

    water_text.innerHTML = water_value + "cm";
    water_Bar.style.height = Math.ceil(100*(1 - (water_value - WATER_MIN) / (WATER_MAX - WATER_MIN))) + "%";  

    var temp_Bar = document.getElementById('temp_adj');
    var temp_text = document.getElementById('temp_value'); 

    temp_text.innerHTML = temp_value + "°C";
    temp_Bar.style.height = Math.ceil(100*(1 - (temp_value - TEMPERATURE_MIN) / (TEMPERATURE_MAX - TEMPERATURE_MIN))) + "%";
  }
  
