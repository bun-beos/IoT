using Dapper;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System.Reflection.Metadata;
using testAPI.Entity;

namespace testAPI.Controllers
{
    [Route("")]
    [ApiController]
    public class ESP32CamsController : ControllerBase
    {
        [HttpGet]
        public IActionResult Get()
        {
            return Ok("Server is running");
        }

        [HttpGet]
        [Route("[controller]")]
        public async Task<List<ImageEntity>> GetAllAsync()
        {
            var connectionString = "Server=127.0.0.1;Port=3306;Database=esp32-cam;Uid=root;Pwd=123456;";
            var connection = new MySqlConnector.MySqlConnection(connectionString);

            var sql = "SELECT * FROM image ORDER BY ImageTime DESC";

            var imageEntities = await connection.QueryAsync<ImageEntity>(sql);

            return imageEntities.ToList();
        }
        
        [HttpGet]
        [Route("[controller]/Latest")]
        public async Task<ImageEntity?> GetLatestAsync()
        {
            var connectionString = "Server=127.0.0.1;Port=3306;Database=esp32-cam;Uid=root;Pwd=123456;";
            var connection = new MySqlConnector.MySqlConnection(connectionString);

            var sql = "SELECT * FROM image ORDER BY ImageTime DESC LIMIT 1";

            var imageEntity = await connection.QueryFirstOrDefaultAsync<ImageEntity>(sql);

            return imageEntity;
        }

        [HttpPost]
        [Route("[controller]")]
        public async Task<int> InsertImageAsync([FromBody] ImageEntity imageEntity)
        {
            var connectionString = "Server=127.0.0.1;Port=3306;Database=esp32-cam;Uid=root;Pwd=123456;";
            var connection = new MySqlConnector.MySqlConnection(connectionString);

            imageEntity.ImageId = Guid.NewGuid();
            var param = new
            {
                imageId = imageEntity.ImageId,
                imageName = imageEntity.ImageName,
                imageTime = imageEntity.ImageTime,
                imageUrl = imageEntity.ImageUrl
            };

            var sql = @"INSERT INTO image (imageId, imageName, imageTime, imageUrl) 
                VALUES (@ImageId, @ImageName, @ImageTime, @ImageUrl)";

            var result = await connection.ExecuteAsync(sql, param);

            return result;
        }

        [HttpGet]
        [Route("[controller]/DoorStatus")]
        public async Task<List<DoorStatusEntity>> GetAllDoorStatusAsync()
        {
            var connectionString = "Server=127.0.0.1;Port=3306;Database=esp32-cam;Uid=root;Pwd=123456;";
            var connection = new MySqlConnector.MySqlConnection(connectionString);

            var sql = "SELECT * FROM doorStatus ORDER BY DoorStatusTime DESC";

            var result = await connection.QueryAsync<DoorStatusEntity>(sql);

            return result.ToList();
        }

        [HttpPost]
        [Route("[controller]/DoorStatus")]
        public async Task<int> InsertDoorStatusAsync([FromBody] DoorStatusEntity doorStatusEntity)
        {
            var connectionString = "Server=127.0.0.1;Port=3306;Database=esp32-cam;Uid=root;Pwd=123456;";
            var connection = new MySqlConnector.MySqlConnection(connectionString);

            doorStatusEntity.DoorStatusId = Guid.NewGuid();
            var param = new
            {
                doorStatusId = doorStatusEntity.DoorStatusId,
                doorStatusTime = doorStatusEntity.DoorStatusTime,
                doorStatusValue = doorStatusEntity.DoorStatusValue,
            };

            var sql = @"INSERT INTO doorStatus (doorStatusId, doorStatusTime, doorStatusValue) 
                VALUES (@DoorStatusId, @DoorStatusTime, @DoorStatusValue)";

            var result = await connection.ExecuteAsync(sql, param);

            return result;
        }
    }
}
