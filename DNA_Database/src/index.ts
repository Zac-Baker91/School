import "reflect-metadata"

import { AppDataSource } from "./data-source"
import csv = require('csv-parser')
import * as fs from "fs"
import { Cell } from "./entity/cell.entity"
import { Interaction } from "./entity/interaction.entity"
import {Locus} from "./entity/locus.entity"
import { EntityManager} from "typeorm"
//import entities and parser nodes

AppDataSource.initialize().then(async () => {

    console.log("Inserting data from files in to DB")

    //const DN_inter=[]
    fs.createReadStream('./Raw data/DN_contacts.txt')
    .pipe(csv({separator: '\t', headers:['chr_l','chr_r']}))
    .on('data',(data)=>{
        AppDataSource
            .createQueryBuilder()
            .insert()
            .into(Locus)
            .values([
                {locus_id: data['chr_l']},
                {locus_id: data:['chr_r']},
            ]).execute()
        
        AppDataSource
            .createQueryBuilder()
            .insert()
            .into(Cell)
            .values([
                {type:"DN"}
            ]).execute();

        AppDataSource
            .createQueryBuilder()
            .relation(Locus,"celltype")
            .of(data['chr_l'])
            .set("DN")
        
        })
        //Interaction might go here

    .on('end',()=>{})

    //const PGN_inter=[]
    fs.createReadStream('./Raw data/PGN_contacts.txt')
    .pipe(csv({separator: '\t', headers:['chr_l','chr_r']}))
    .on('data',(data)=>{
        const locus = AppDataSource
        .createQueryBuilder()
        .insert()
        .into(Tran)
        .values([
            {locus_id:data['chr_l']},{locus_id:['chr_r']}
        ]).execute()
    
    const cell = AppDataSource
        .createQueryBuilder()
        .insert()
        .into(Cell)
        .values([
            {type:"PGN"}
        ]).execute();

    AppDataSource
        .createQueryBuilder()
        .relation(Locus,"celltype")
        .of(data['chr_l'])
        .set("PGN")

    })
    .on('end',()=>{})

    //const gene_exp=[]
    fs.createReadStream('./Raw data/UTF-8"Expression_in_PGN_and_DN.csv')
    .pipe(csv())
    .on('data',(data)=>{

    })
    .on('end',()=>{})
    
    //const motif_dis=[]
    fs.createReadStream('./Raw data/Motif_info.tsv')
    .pipe(csv({separator: '\t'}))
    .on('data',(data)=>{

    })
    .on('end',()=>{})

    //const PGN_M_Inst=[]
    fs.createReadStream('./Raw data/PGN_motif_window_intersect.bed')
    .pipe(csv({separator: '\t', headers: ["chr", "l_start", "l_end", "l_id", "chr_dup", "m_start", "m_end", "motif", "theshold", "f/v"]}))
    .on('data',(data)=>{

    })
    .on('end',()=>{})

    //const DN_M_Inst=[]
    fs.createReadStream('./Raw data/DN_motif_window_intersect.bed')
    .pipe(csv({separator: '\t', headers: ["chr", "l_start", "l_end", "l_id", "chr_dup", "m_start", "m_end", "motif", "theshold", "f/v"]}))
    .on('data',(data)=>{

    })
    .on('end',()=>{})




    //Implement parser and load to data base
    //user.firstName = "Timber"
    //user.lastName = "Saw"
    //user.age = 25
    //await AppDataSource.manager.save(user)
    //console.log("Saved a new user with id: " + user.id)

    console.log("Loading entities and objects from the database...")

    const cell = AppDataSource.manager.find(Cell)
    console.log("Loaded users: ", cell)

    const PGNcell =  AppDataSource
    .getRepository(Cell)
    .createQueryBuilder("PGN_cell")
    .where("PGN_cell.type = :type", { tpye: "PGN" })
    .getOne()
    console.log("PGN cell: ",PGNcell)

    //SELECT
    //  PGN_cell.type as type ,
    //  PGN_cell.intructions as intruction,
    //FROM cell PGN_cell
    //WHERE PGN_cell.type = "PGN"

    //raw SQL
    //   const SQLq = await AppDataSource.query()





    console.log("Here you can setup and run express / fastify / any other framework.")

    //Possible UI stuff

}).catch(error => console.log(error))
