<template>
  <div id="app">
    <span>Eufa.add({{ add.params.x }}, {{ add.params.y }}): {{ add.result }}</span>
  </div>
</template>

<script>
export default {
  name: 'app',
  data () {
    return {
      eufa: null,
      add: {
        params: {
          x: 10,
          y: 20
        },
        result: ''
      }
    }
  },
  created () {
    window.Eufa.init.install('static/eufa.wasm', 'static/eufa.js', () => {
      // Call directly
      this.add.result = window.Eufa.add(this.add.params.x, this.add.params.y)
    })
  }
}

</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
