<template>
  <div id="app">
    <h1>Eufa</h1>
    <h4>A high efficient utility functions library written in webassembly.</h4>
    <br/>
    <div>
      <h3>Math</h3>
      <table>
        <tr>
          <th width="680">Method</th>
          <th>Result</th>
        </tr>
        <tr>
          <td>Eufa.Math.i64_add({{ params.i_x }}, {{ params.i_y }})</td>
          <td>{{ i64_add }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.f64_add({{ params.f_x }}, {{ params.f_y }})</td>
          <td>{{ f64_add }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.i64_minus({{ params.i_x }}, {{ params.i_y }})</td>
          <td>{{ i64_minus }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.f64_minus({{ params.f_x }}, {{ params.f_y }})</td>
          <td>{{ f64_minus }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.i64_multiply({{ params.i_x }}, {{ params.i_y }})</td>
          <td>{{ i64_multiply }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.f64_multiply({{ params.f_x }}, {{ params.f_y }})</td>
          <td>{{ f64_multiply }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.i64_divide({{ params.i_x }}, {{ params.i_y }})</td>
          <td>{{ i64_divide }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.f64_divide({{ params.f_x }}, {{ params.f_y }})</td>
          <td>{{ f64_divide }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.i64_abs({{ params.ni_x }})</td>
          <td>{{ i64_abs }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.f64_abs({{ params.nf_x }})</td>
          <td>{{ f64_abs }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.i64_sqrt({{ params.i_x }})</td>
          <td>{{ i64_sqrt }}</td>
        </tr>
        <tr>
          <td>Eufa.Math.f64_sqrt({{ params.f_x }})</td>
          <td>{{ f64_sqrt }}</td>
        </tr>
      </table>

      <h3>String</h3>
      <table>
        <tr>
          <th width="680">Method</th>
          <th>Result</th>
        </tr>
        <tr>
          <td>Eufa.String.capitalize('{{ params.string_en }}')</td>
          <td>{{ ptr_capitalize }}</td>
        </tr>
      </table>

      <h3>Array</h3>
      <table>
        <tr>
          <th width="680">Method</th>
          <th>Result</th>
        </tr>
        <tr>
          <td>Eufa.Array.num_sort({{ params.array_num }})</td>
          <td>{{ num_sort }}</td>
        </tr>
        <tr>
          <td>Eufa.Array.num_rsort({{ params.array_num }})</td>
          <td>{{ num_rsort }}</td>
        </tr>
      </table>

      <h3>Cache</h3>
      <table>
        <tr>
          <th width="680">Method</th>
          <th>Result</th>
        </tr>
        <tr>
          <td>Eufa.Cache.get('{{ params.cache_num_key }}')</td>
          <td>{{ get_num }}</td>
        </tr>
        <tr>
          <td>Eufa.Cache.get('{{ params.cache_str_key }}')</td>
          <td>{{ get_str }}</td>
        </tr>
        <tr>
          <td>Eufa.Cache.get('{{ params.cache_obj_key }}')</td>
          <td>{{ get_obj }}</td>
        </tr>
        <tr>
          <td>Eufa.Cache.get('{{ params.cache_arr_key }}')</td>
          <td>{{ get_arr }}</td>
        </tr>
        <tr>
          <td>Eufa.Cache.get('{{ params.cache_none_key }}')</td>
          <td>(type) {{ get_none }}</td>
        </tr>
      </table>

      <h3>Encryptor</h3>
      <table>
        <tr>
          <th width="680">Method</th>
          <th>Result</th>
        </tr>
        <tr>
          <td>Eufa.Encryptor.base64_encode('{{ params.string_zh }}')</td>
          <td>{{ base64_encode_zh }}</td>
        </tr>
        <tr>
          <td>Eufa.Encryptor.base64_encode('{{ params.string_en }}')</td>
          <td>{{ base64_encode_en }}</td>
        </tr>
        <tr>
          <td>Eufa.Encryptor.base64_decode('{{ params.string_zh_base64_encoded }}')</td>
          <td>{{ base64_decode_zh }}</td>
        </tr>
        <tr>
          <td>Eufa.Encryptor.base64_decode('{{ params.string_en_base64_encoded }}')</td>
          <td>{{ base64_decode_en }}</td>
        </tr>
        <tr>
          <td>Eufa.Encryptor.md5('{{ params.string_en }}')</td>
          <td>{{ md5_en }}</td>
        </tr>
        <tr>
          <td>Eufa.Encryptor.md5('{{ params.string_zh }}')</td>
          <td>{{ md5_zh }}</td>
        </tr>
        <tr>
          <td>Eufa.Encryptor.sha1('{{ params.string_en }}')</td>
          <td>{{ sha1_en }}</td>
        </tr>
        <tr>
          <td>Eufa.Encryptor.sha1('{{ params.string_zh }}')</td>
          <td>{{ sha1_zh }}</td>
        </tr>
      </table>

      <h3>DLib</h3>
      <table>
        <tr>
          <th width="380">Method</th>
          <th>Input</th>
          <th width="400">Result</th>
        </tr>
        <tr>
          <td>Eufa.DLib.testcase_kmeans()</td>
          <td>None</td>
          <td><div class="chart" ref="eufa_dlib_testcase_kmeans"></div></td>
        </tr>
          <td>Eufa.DLib.testcase_dnn_mnist()</td>
          <td><img src="/static/testcases/mnist_test_6.bmp"/></td>
          <td>{{ dlib_dnn_mnist }}</td>
        </tr>
      </table>
    </div>
  </div>
</template>

<script>
import eufa from 'eufa'
import echarts from 'echarts'

export default {
  name: 'app',
  data () {
    return {
      params: {
        i_x: 10,
        i_y: 20,
        f_x: 11.5,
        f_y: 22.5,
        ni_x: -10,
        ni_y: -20,
        nf_x: -11.5,
        nf_y: -22.5,
        string_en: 'Hello, WebAssembly :)',
        string_zh: '你好，WebAssembly ：）',
        string_en_base64_encoded: 'SGVsbG8sIFdlYkFzc2VtYmx5IDopAA==',
        string_zh_base64_encoded: '5L2g5aW977yMV2ViQXNzZW1ibHkg77ya77yJAA==',
        array_num: [5, 3, 6, 0.2, 5, 9],
        cache_num_key: 'EUFA_CACHE_NUM_KEY',
        cache_num_val: 115655.49806892683,
        cache_str_key: 'EUFA_CACHE_STR_KEY',
        cache_str_val: 'EUFA_CACHE_STR_VAL',
        cache_obj_key: 'EUFA_CACHE_OBJ_KEY',
        cache_obj_val: {
          'EUFA_CACHE_OBJ_KEY': 'EUFA_CACHE_OBJ_VAL'
        },
        cache_arr_key: 'EUFA_CACHE_ARR_KEY',
        cache_arr_val: ['EUFA_CACHE_ARR_KEY', 'EUFA_CACHE_ARR_VAL'],
        cache_none_key: 'EUFA_CACHE_NONE_KEY'
      },
      eufa: null,
      i64_add: '',
      f64_add: '',
      i64_minus: '',
      f64_minus: '',
      i64_multiply: '',
      f64_multiply: '',
      i64_divide: '',
      f64_divide: '',
      i64_abs: '',
      f64_abs: '',
      i64_sqrt: '',
      f64_sqrt: '',
      ptr_capitalize: '',
      base64_encode_zh: '',
      base64_encode_en: '',
      base64_decode_zh: '',
      base64_decode_en: '',
      md5_en: '',
      md5_zh: '',
      sha1_en: '',
      sha1_zh: '',
      num_sort: '',
      num_rsort: '',
      mlpack_version: '',
      get_num: '',
      get_str: '',
      get_obj: '',
      get_arr: '',
      get_none: '',
      dlib_dnn_mnist: ''
    }
  },
  created () {
    eufa.init({
      wasmStaticDir: 'static/'
    }, () => {
      console.log(eufa)
      window.Eufa = eufa
      // Call directly
      // Math.add
      this.i64_add = eufa.Math.i64_add(this.params.i_x, this.params.i_y)
      this.f64_add = eufa.Math.f64_add(this.params.f_x, this.params.f_y)
      // Math.minus
      this.i64_minus = eufa.Math.i64_minus(this.params.i_x, this.params.i_y)
      this.f64_minus = eufa.Math.f64_minus(this.params.f_x, this.params.f_y)
      // Math.multiply
      this.i64_multiply = eufa.Math.i64_multiply(this.params.i_x, this.params.i_y)
      this.f64_multiply = eufa.Math.f64_multiply(this.params.f_x, this.params.f_y)
      // Math.divide
      this.i64_divide = eufa.Math.i64_divide(this.params.i_y, this.params.i_x)
      this.f64_divide = eufa.Math.f64_divide(this.params.f_x, this.params.f_y)
      // Math.abs
      this.i64_abs = eufa.Math.i64_abs(this.params.ni_x)
      this.f64_abs = eufa.Math.f64_abs(this.params.nf_x)
      // Math.sqrt
      this.i64_sqrt = eufa.Math.i64_sqrt(this.params.i_x)
      this.f64_sqrt = eufa.Math.f64_sqrt(this.params.f_x)
      // String.capitalize
      this.ptr_capitalize = eufa.String.capitalize(this.params.string_en)
      // Encryptor.base64_encode
      this.base64_encode_zh = eufa.Encryptor.base64_encode(this.params.string_zh)
      this.base64_encode_en = eufa.Encryptor.base64_encode(this.params.string_en)
      this.base64_decode_zh = eufa.Encryptor.base64_decode(this.params.string_zh_base64_encoded)
      this.base64_decode_en = eufa.Encryptor.base64_decode(this.params.string_en_base64_encoded)
      // Encryptor.md5
      this.md5_en = eufa.Encryptor.md5(this.params.string_en)
      this.md5_zh = eufa.Encryptor.md5(this.params.string_zh)
      // Encryptor.sha1
      this.sha1_en = eufa.Encryptor.sha1(this.params.string_en)
      this.sha1_zh = eufa.Encryptor.sha1(this.params.string_zh)
      // Array.num_sort
      this.num_sort = eufa.Array.num_sort(this.params.array_num)
      // Array.num_rsort
      this.num_rsort = eufa.Array.num_rsort(this.params.array_num)
      // Cache.set
      eufa.Cache.set(this.params.cache_num_key, this.params.cache_num_val)
      eufa.Cache.set(this.params.cache_str_key, this.params.cache_str_val)
      eufa.Cache.set(this.params.cache_obj_key, this.params.cache_obj_val)
      eufa.Cache.set(this.params.cache_arr_key, this.params.cache_arr_val)
      // Cache.get
      this.get_num = eufa.Cache.get(this.params.cache_num_key)
      this.get_str = eufa.Cache.get(this.params.cache_str_key)
      this.get_obj = eufa.Cache.get(this.params.cache_obj_key)
      this.get_arr = eufa.Cache.get(this.params.cache_arr_key)
      this.get_none = Object.prototype.toString.call(eufa.Cache.get(this.params.cache_none_key))
      // DLib.testcase_kmeans
      let kmeansPayload = JSON.parse(eufa.DLib.testcase_kmeans())
      let kmeansSeriesData = {}
      let kmeansSeries = []
      let kmeansLabel = new Set()
      kmeansPayload.forEach(row => {
        kmeansLabel.add(`C${row[0]}`)
        if (!kmeansSeriesData[`d${row[0]}`]) {
          kmeansSeriesData[`d${row[0]}`] = []
        }
        kmeansSeriesData[`d${row[0]}`].push([row[1], row[2]])
      })
      Array.from(kmeansLabel).forEach((row, index) => {
        kmeansSeries.push({
          name: `C${index}`,
          type: 'scatter',
          data: kmeansSeriesData[`d${index}`]
        })
      })
      echarts.init(this.$refs.eufa_dlib_testcase_kmeans).setOption({
        title: {
          text: 'K-Means'
        },
        grid: {
          left: '3%',
          right: '7%',
          bottom: '3%',
          containLabel: true
        },
        tooltip: {
          showDelay: 0,
          formatter: function (params) {
            return params.seriesName + ' :<br/>(' + params.value[0] + ', ' + params.value[1] + ')'
          },
          axisPointer: {
            show: true,
            type: 'cross',
            lineStyle: {
              type: 'dashed',
              width: 1
            }
          }
        },
        legend: {
          data: Array.from(kmeansLabel),
          left: 'center'
        },
        xAxis: [
          {
            type: 'value',
            scale: true,
            axisLabel: {
              formatter: '{value}'
            },
            splitLine: {
              show: false
            }
          }
        ],
        yAxis: [
          {
            type: 'value',
            scale: true,
            axisLabel: {
              formatter: '{value}'
            },
            splitLine: {
              show: false
            }
          }
        ],
        series: kmeansSeries
      })
      // DLib.testcase_dnn_mnist
      let img = new Image()
      img.src = '/static/testcases/mnist_test_6.bmp'
      img.onload = () => {
        let canvas = document.createElement('canvas')
        canvas.width = img.width
        canvas.height = img.height
        canvas.getContext('2d').drawImage(img, 0, 0, img.width, img.height)
        let pixelData = canvas.getContext('2d').getImageData(0, 0, img.width, img.height)
        this.dlib_dnn_mnist = eufa.DLib.testcase_dnn(pixelData.data)
      }
    })
  }
}

</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: left;
  color: #2c3e50;
  margin-top: 60px;
}

table {
  width: 1200px;
  border-collapse: collapse;
  border: none;
}
table td, table th {
  border: solid #000 1px;
  padding: 5px;
  text-align: left;
}

.chart{
  height: 300px;
}
</style>
