"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g;
    return g = { next: verb(0), "throw": verb(1), "return": verb(2) }, typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (_) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
Object.defineProperty(exports, "__esModule", { value: true });
var data_source_1 = require("./data-source");
var csv = require("csv-parser");
var fs = require("fs");
var cell_entity_1 = require("./entity/cell.entity");
var locus_entity_1 = require("./entity/locus.entity");
//import entities and parser nodes
data_source_1.AppDataSource.initialize().then(function () { return __awaiter(void 0, void 0, void 0, function () {
    var cell, PGNcell;
    return __generator(this, function (_a) {
        console.log("Inserting data from files in to DB");
        //const DN_inter=[]
        fs.createReadStream('./Raw data/DN_contacts.txt')
            .pipe(csv({ separator: '\t', headers: ['chr_l', 'chr_r'] }))
            .on('data', function (data) {
            data_source_1.AppDataSource
                .createQueryBuilder()
                .insert()
                .into(locus_entity_1.Locus)
                .values([
                { locus_id: data['chr_l'] }, { locus_id: ['chr_r'] }
            ]).execute();
            data_source_1.AppDataSource
                .createQueryBuilder()
                .insert()
                .into(cell_entity_1.Cell)
                .values([
                { type: "DN" }
            ]).execute();
            data_source_1.AppDataSource
                .createQueryBuilder()
                .relation(locus_entity_1.Locus, "celltype")
                .of(data['chr_l'])
                .set("DN");
        })
            //Interaction might go here
            .on('end', function () { });
        //const PGN_inter=[]
        fs.createReadStream('./Raw data/PGN_contacts.txt')
            .pipe(csv({ separator: '\t', headers: ['chr_l', 'chr_r'] }))
            .on('data', function (data) {
            var locus = data_source_1.AppDataSource
                .createQueryBuilder()
                .insert()
                .into(locus_entity_1.Locus)
                .values([
                { locus_id: data['chr_l'] }, { locus_id: ['chr_r'] }
            ]).execute();
            var cell = data_source_1.AppDataSource
                .createQueryBuilder()
                .insert()
                .into(cell_entity_1.Cell)
                .values([
                { type: "PGN" }
            ]).execute();
            data_source_1.AppDataSource
                .createQueryBuilder()
                .relation(locus_entity_1.Locus, "celltype")
                .of(data['chr_l'])
                .set("PGN");
        })
            .on('end', function () { });
        //const gene_exp=[]
        fs.createReadStream('./Raw data/UTF-8"Expression_in_PGN_and_DN.csv')
            .pipe(csv())
            .on('data', function (data) {
        })
            .on('end', function () { });
        //const motif_dis=[]
        fs.createReadStream('./Raw data/Motif_info.tsv')
            .pipe(csv({ separator: '\t' }))
            .on('data', function (data) {
        })
            .on('end', function () { });
        //const PGN_M_Inst=[]
        fs.createReadStream('./Raw data/PGN_motif_window_intersect.bed')
            .pipe(csv({ separator: '\t', headers: ["chr", "l_start", "l_end", "l_id", "chr_dup", "m_start", "m_end", "motif", "theshold", "f/v"] }))
            .on('data', function (data) {
        })
            .on('end', function () { });
        //const DN_M_Inst=[]
        fs.createReadStream('./Raw data/DN_motif_window_intersect.bed')
            .pipe(csv({ separator: '\t', headers: ["chr", "l_start", "l_end", "l_id", "chr_dup", "m_start", "m_end", "motif", "theshold", "f/v"] }))
            .on('data', function (data) {
        })
            .on('end', function () { });
        //Implement parser and load to data base
        //user.firstName = "Timber"
        //user.lastName = "Saw"
        //user.age = 25
        //await AppDataSource.manager.save(user)
        //console.log("Saved a new user with id: " + user.id)
        console.log("Loading entities and objects from the database...");
        cell = data_source_1.AppDataSource.manager.find(cell_entity_1.Cell);
        console.log("Loaded users: ", cell);
        PGNcell = data_source_1.AppDataSource
            .getRepository(cell_entity_1.Cell)
            .createQueryBuilder("PGN_cell")
            .where("PGN_cell.type = :type", { tpye: "PGN" })
            .getOne();
        console.log("PGN cell: ", PGNcell);
        //SELECT
        //  PGN_cell.type as type ,
        //  PGN_cell.intructions as intruction,
        //FROM cell PGN_cell
        //WHERE PGN_cell.type = "PGN"
        console.log("Here you can setup and run express / fastify / any other framework.");
        return [2 /*return*/];
    });
}); }).catch(function (error) { return console.log(error); });
//# sourceMappingURL=index.js.map